all: Softphone.Framework

clean:
	-rm -rf libsoftphone

libsoftphone: libsoftphone*.zip
	-rm -rf libsoftphone
	unzip -qq libsoftphone*.zip -d libsoftphone;
	cp -R libsoftphone/Frameworks/ali.framework libsoftphone/ali.framework;

ali.Framework: libsoftphone
	cp -R libsoftphone/Frameworks/ali.framework libsoftphone/ali.framework

Softphone.Framework: libsoftphone ali.Framework
	cp -Rf libsoftphone/Frameworks/${CONFIGURATION}/Softphone.framework libsoftphone/Softphone.framework
