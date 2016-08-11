#/bin/sh

./configure --host=arm-linux-androideabi \
--with-ssl=/usr/local/ssl/android-9\
--with-nghttp2=/usr/local \
--disable-ftp \
--disable-gopher \
--disable-file \
--disable-imap \
--disable-ldap \
--disable-ldaps \
--disable-pop3 \
--disable-proxy \
--disable-rtsp \
--disable-smtp \
--disable-telnet \
--disable-tftp \
--without-gnutls \
--without-libidn \
--without-librtmp \
--disable-dict
