/*
 *  BalanceRecord.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 4/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ali_string.h"
#include "ali_auto_ptr.h"

namespace ali
{
    namespace xml
    {
        struct tree;
    }
}

namespace Balance
{
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	struct Record
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
		Record();
		~Record();

        Record(Record const& other);
        Record & operator =(Record const& other);
    public:
		bool valid;
		
		ali::string balanceString;
		float balance;

        ali::xml::tree & info();
        ali::xml::tree const& info() const;
        
    private:
        mutable ali::auto_ptr<ali::xml::tree> _info;
	};
}

namespace ali
{
    template <typename T>
    struct format_t;

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    template <>
    struct format_t<Balance::Record>
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        ali::string operator ()(Balance::Record const& record) const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return record.balanceString;
        }
    };
}
