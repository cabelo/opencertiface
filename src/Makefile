VERSION     = 1.0.0
RELEASE     = RC2
CC          = gcc -DLINUX
#CXX         = g++
OBJSBIN1    = webServerVerifyFace.o       soapServer.o soapC.o functionSRV.o  ping.o verifyAll.o 
OBJSBIN2    = webServerVerifyFaceThread.o soapServer.o soapC.o functionSRV.o  ping.o verifyAll.o
LIBS1       = -lgsoap
LIBS2       = -lgsoap -lstdc++ -lopencv_core -lopencv_highgui  -lopenbr -lQt5Core
LIBS3       = -lgsoap -lstdc++ -lpthread -lopencv_core -lopencv_highgui  -lopenbr -lQt5Core
 
LIBSFACE    = 
CXXFLAGS    = -fpermissive
INCLUDE     = -I/usr/share/gsoap/ -I/usr/share/gsoap/plugin 
INCLUDEFACE = 
IFACE       = -pipe -O2 -W -D_REENTRANT -fPIE -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -I/usr/include/opencv

all: webServerVerifyFace webServerVerifyFaceThread clientPING clientVerifyImg

clientPING: clientPING.o soapC.o soapClient.o
	$(CC) -DLINUX -g -O2  clientPING.o soapClient.o soapC.o  -lgsoap  -o $@ 

clientVerifyImg: clientVerifyImg.o soapC.o soapClient.o
	$(CC) -DLINUX -g -O2  clientVerifyImg.o soapClient.o soapC.o  -lgsoap  -o $@


webServerVerifyFace: $(OBJSBIN1)
	$(CC) -DLINUX -g -O2  $(OBJSBIN1) $(LIBS3) $(DIRLIBSDK) $(LIBSFACE) -o $@ 

webServerVerifyFaceThread: $(OBJSBIN2)
	$(CC) -DLINUX -g -O2  $(OBJSBIN2) $(LIBS3) $(DIRLIBSDK) $(LIBSFACE) -o $@ 




soapClient.o: soapClient.c 
	$(CC) -DLINUX -DWITH_NOEMPTYSTRUCT -c $(INCLUDE) $< -o $@

soapServer.o: soapServer.c
	$(CC) -DLINUX -DWITH_NOEMPTYSTRUCT -c $(INCLUDE) $< -o $@

soapC.o: soapC.c
	$(CC) -DLINUX -DWITH_NOEMPTYSTRUCT -c $(INCLUDE) $< -o $@

verifyAll.o: verifyAll.cpp 
	$(CC) $(IFACE) -DLINUX -DWITH_NOEMPTYSTRUCT -c $(INCLUDE)   $< -o $@

ping.o: ping.c
	$(CC) -DLINUX -DWITH_NOEMPTYSTRUCT -c $(INCLUDE) $< -o $@

functionSRV.o: functionSRV.c
	$(CC) -DLINUX -DWITH_NOEMPTYSTRUCT -c $(INCLUDE) $< -o $@

clean:
	rm *.o webServerVerifyFace webServerVerifyFaceThread clientPING  clientVerifyImg

install:
	@if test -f /etc/init.d/opencertiface; then \
		/etc/init.d/opencertiface stop; \
	fi
	install -m 755 -c webServerVerifyFaceThread /usr/sbin/opencertiface
	install -m 755 -c script/opencertiface /etc/init.d/opencertiface
	ln -sf  /etc/init.d/opencertiface /usr/sbin/rcopencertiface
	@if test -f /bin/systemctl; then \
	   systemctl daemon-reload; \
	fi
	/etc/init.d/opencertiface start

uninstall:
	/etc/init.d/opencertiface stop
	rm  -f /usr/sbin/opencertiface
	rm  -f /etc/init.d/opencertiface
	rm  -rf /usr/sbin/opencertiface


