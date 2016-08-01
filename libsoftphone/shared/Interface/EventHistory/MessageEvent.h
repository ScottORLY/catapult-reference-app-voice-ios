/*
 *  EventHistory/MessageEvent.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2016 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "EventHistory/EventHistory.h"
#include "ali_optional.h"
#include "ali_mime_content_type.h"
#include "ali_callback.h"

namespace Softphone
{
    namespace EventHistory
    {
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct MessageResult
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {

            enum Type
            {
                Unknown             = 0,
                Initial             = 1 << 0,
                Sending             = 1 << 1,
                SendFailed          = 1 << 2,
                SendSuccessful      = 1 << 3,

                max                 = SendSuccessful,
                all                 = (max << 1) - 1
            };

            static ali::string toString(int result);
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct GenericMessageErrorCode
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            enum Type
            {
                NoError             = 0,
                NoNetwork           = 1,
                NoWifi              = 2,
                NoTransport         = 3,
                Busy                = 4,
                AccountNotFound     = 5,
                TransportFailed     = 6
            };

            static ali::string toString(int result);
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct MessageAttachment
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            friend class MessageEvent;

        public:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageAttachment()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageAttachment(ali::mime::content_type contentType,
                              ali::filesystem2::path path);
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageAttachment(ali::string_const_ref key,
                              ali::mime::content_type contentType,
                              ali::filesystem2::path path);
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageAttachment(ali::mime::content_type contentType,
                              Attribute::Value path);
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageAttachment(ali::string_const_ref key,
                              ali::mime::content_type contentType,
                              Attribute::Value path)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : mKey(key)
                , contentType(ali::move(contentType))
                , path(ali::move(path))
            {}


            ali::string const& getKey() const {return mKey;}
            ali::filesystem2::path getPath() const {return expandFile(path);}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::optional<ali::filesystem2::path> getPreviewPath() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return previewPath.is_null()
                        ? ali::optional<ali::filesystem2::path>()
                        : expandFile(*previewPath);
            }

        private:
            ali::string mKey;

        public:
            ali::mime::content_type contentType;
            Attribute::Value path;                       // may not be expanded

            // TODO One object for preview
            ali::optional<int> previewWidth;            // preview image metadata
            ali::optional<int> previewHeight;
            ali::optional<Attribute::Value> previewPath;

            ali::optional<ali::string> contentUrl;              // if the content can be downloaded from the server
            ali::optional<ali::string> filename;                // user-friendly filename
        };

        typedef ali::array<MessageAttachment> MessageAttachmentsType;

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        class MessageEvent
            : public Event
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
        public:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct Subtype
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                enum Type
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                {
                    Unknown     = 0,
                    IM,
                    SMS,
                    MMS
                };
            };

        public:
            typedef EventPointer<MessageEvent> Pointer;
            typedef ali::string ContentDownloadHandle;

            class QueryBuilder;
            struct AttachmentInfo;
            struct AttachmentProgressInfo;
            struct AttachmentStatusInfo;
            
            typedef ali::callback<void(AttachmentStatusInfo)> ContentDownloadStatusCallback;
            typedef ali::callback<void(AttachmentProgressInfo)> ContentDownloadProgressCallback;
            
            typedef ContentDownloadStatusCallback ContentUploadStatusCallback;
            typedef ContentDownloadProgressCallback ContentUploadProgressCallback;
            
            static const EventHistory::EventType::Type EventType = EventHistory::EventType::Message;

        protected:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageEvent()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : Event(EventType)
            {}

        public:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            static Pointer create()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return Pointer(new MessageEvent());
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getSubject() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::subject);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setSubject(ali::string_const_ref subject)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::subject, subject);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getBody() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::body);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setBody(ali::string_const_ref body)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::body, body);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getTransportId() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::transportId);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setTransportId(ali::string_const_ref transportId)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::transportId, transportId);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getPreviewText() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::previewText);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setPreviewText(ali::string_const_ref text)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::previewText, text);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getEffectiveContentType() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::effectiveContentType);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setEffectiveContentType(ali::string_const_ref text)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::effectiveContentType, text);
            }

            GenericMessageErrorCode::Type getGenericErrorCode() const;

            Subtype::Type getSubtype() const;
            MessageResult::Type getMessageResult() const;
            void setMessageResult(MessageResult::Type result);
            void clearGenericErrorCode();
            void failWithGenericErrorCode(GenericMessageErrorCode::Type code);

            int getAttachmentCount() const;
            MessageAttachment getAttachment(ali::string_const_ref key) const;
            MessageAttachmentsType getAttachments() const;
            void addAttachment(MessageAttachment const& attachment);
            void removeAttachment(ali::string_const_ref key);

            void setAttachmentPreviewSize(ali::string_const_ref key, int w, int h);

            bool isRemoteUserSuccessful(int i) const;
            bool isRemoteUserSuccessful(ali::generic_peer_address const& addr) const;
            bool setRemoteUserSending(int i);
            bool setRemoteUserSent(ali::generic_peer_address const& addr);
            bool setRecipientFailed(ali::generic_peer_address const& addr);
            bool setRemoteUserFailedByExternalId(ali::string_const_ref externalMessageId);
            bool setRemoteUserDeliveredByExternalId(ali::string_const_ref externalMessageId);
            bool setRemoteUserReadByExternalId(ali::string_const_ref externalMessageId);
            bool setExternalIdForRecipient(ali::string_const_ref externalId, ali::generic_peer_address const& remoteTransportUri);
            void updateMessageResult();
            bool setRecipientAttribute(int i, ali::string_const_ref key, ali::string_const_ref value);
            bool setRecipientAttribute(ali::generic_peer_address const& addr, ali::string_const_ref key, ali::string_const_ref value);
            void updateStream(MessageResult::Type result);

        private:
            int getSuccessfulRecipientCount() const;
            int getRemoteUserPositionByAddress(ali::generic_peer_address const& address, int startingIndex) const;
            int getRemoteUserPositionByExternalId(ali::string_const_ref externalId, int startingIndex) const;

        public:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct Attributes
                : public Event::Attributes
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                static const ali::string_literal messageResult;
                static const ali::string_literal subject;
                static const ali::string_literal body;
                static const ali::string_literal subtype;
                static const ali::string_literal sipisTimestamp;
                static const ali::string_literal previewText;
                static const ali::string_literal effectiveContentType;
                static const ali::string_literal transportId;
                static const ali::string_literal localizedBodyKey;
                static const ali::string_literal localizedBodyKeyDefault;
                static const ali::string_literal localizedStringParameterPrefix;
                static const ali::string_literal localizedNumberParameterPrefix;
                static const ali::string_literal genericErrorCode;
                static const ali::string_literal contentType;

            };
        };

        bool Event::isMessage() const { return eventType == EventType::Message; }
        MessageEvent & Event::asMessage() { ali_assert(isMessage()); return static_cast<MessageEvent &>(*this); }
        MessageEvent const& Event::asMessage() const { ali_assert(isMessage()); return static_cast<MessageEvent const&>(*this); }

        struct Query;

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        class MessageEvent::QueryBuilder
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
        public:
            QueryBuilder(Query & query)
            : mQuery(query){}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            Query & get()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return mQuery;
            }

            QueryBuilder & setMessageResultMask(int mask);

        private:
            Query & mQuery;
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct MessageEvent::AttachmentInfo
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            AttachmentInfo(Softphone::EventHistory::MessageEvent::Pointer const& message,
                           ali::string_const_ref key)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : message(message)
                , key(key)
            {}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            static ContentDownloadHandle makeDownloadHandle(Softphone::EventHistory::MessageEvent::Pointer msg,
                                                            ali::string_const_ref key)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return ali::str::format(msg->getEventId()) + ':' + key;
            }

            Softphone::EventHistory::MessageEvent::Pointer const& message;
            ali::string_const_ref   key;
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct MessageEvent::AttachmentStatusInfo : MessageEvent::AttachmentInfo
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            AttachmentStatusInfo(Softphone::EventHistory::MessageEvent::Pointer const& message,
                                 ali::string_const_ref key,
                                 bool available)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : AttachmentInfo(message, key)
                , available(available)
            {}

            bool                    available;
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct MessageEvent::AttachmentProgressInfo : MessageEvent::AttachmentInfo
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            AttachmentProgressInfo(Softphone::EventHistory::MessageEvent::Pointer const& message,
                                   ali::string_const_ref key,
                                   ali::int64 progress,
                                   ali::int64 total)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : AttachmentInfo(message, key)
                , progress(progress)
                , total(total)
            {}

            ali::int64                    progress;
            ali::int64                    total;
        };
    }
}
