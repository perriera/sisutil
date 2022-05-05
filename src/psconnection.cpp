// PrologServer_Connection.cpp - IPC class for Prolog rule processing engine
// $Id: psconnection.cpp,v 1.1 2000/09/28 15:54:01 ed Exp $

#include <sisutil/psconnection.hpp> 
#include "quintus_tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sisutil/YUBIMErr.hpp>
#include <cstring>       // for memset & strlen
#include <errno.h>

YUError PrologServer_Connection::send(const char* c) {
	YUError mystatus;
	if (!isConnected) {
		mystatus = connect();
		if (!mystatus.isSuccess())
			return mystatus;
	}
	if (getenv("PrologServer_DEBUG")) std::cerr << "PS_SEND:" << c << std::endl;
	//size_t wsize =
	write(myConnection, c, strlen(c));
	return mystatus;
}

YUError
PrologServer_Connection::transceive(const std::string& buf, char* ret, long size) {
	//PrologServer_Connection::transceive(const char* buf, char* ret, long size) {
	YUError mystatus;
	if (!isConnected) {
		mystatus = connect();
		if (!mystatus.isSuccess())
			return mystatus;
	}

	if (getenv("PrologServer_DEBUG")) {
		std::cerr << "PrologServer_Connection::transceive() incoming: " << buf << std::endl;
		//for (int i = 0; i < 85; i++) {
		//   std::cerr << buf[i];
	   //}
		//std::cerr << std::endl;
	}
	//buf = "prolog(display_call_stats).\nterm(end_of_file).";  // temporary - for testing
	//if (getenv("PrologServer_DEBUG")) std::cerr << "PrologServer_Connection::transceive() substituted: " << buf << std::endl;

	 //ssize_t wsize = write(myConnection, buf, strlen(buf));
	ssize_t wsize = write(myConnection, buf.c_str(), buf.length());
	if (getenv("PrologServer_DEBUG")) std::cerr << "PrologServer_Connection::transceive() write(): " << wsize << " bytes" << std::endl;
	if (wsize == -1) {
		std::cerr << "PrologServer_Connection::transceive: write() error: " << errno << std::endl;
		// TODO: return a YUError on failure
	}
	myrfile = fdopen(myConnection, "r");
	if (getenv("PrologServer_DEBUG")) std::cerr << "PrologServer_Connection::transceive() fdopen(): " << myrfile << std::endl;
	if (myrfile == NULL) {
		std::cerr << "PrologServer_Connection::transceive: fdopen() error: " << errno << std::endl;
		// TODO: return a YUError on failure
	}
	memset(ret, 0x00, size);
	size_t rsize = fread(ret, sizeof(char), size, myrfile);
	if (feof(myrfile)) {
		std::cerr << "PrologServer_Connection::transceive(): feof() end of file." << std::endl;
		clearerr(myrfile);
	}
	else if (ferror(myrfile)) {
		std::cerr << "PrologServer_Connection::transceive(): ferror() error." << std::endl;
		clearerr(myrfile);
		// TODO: return a YUError on failure
	}
	if (getenv("PrologServer_DEBUG")) std::cerr << "PrologServer_Connection::transceive(): |" << ret << "|, bytes read: " << rsize << std::endl;
	return mystatus;
}

YUError PrologServer_Connection::connect() {
	YUError mystatus;
	if (!isConnected) {
		//char *filename = getenv(myServer.c_str());  // FIXME
		char* filename = getenv("CAS_SERVER_FILE");
		std::ifstream f(filename);
		char dummy[100];
		char host[100];
		int  port;
		f >> dummy >> port;
		f >> dummy >> host;
		if (getenv("PrologServer_DEBUG")) {
			std::cerr << "PrologServer_Connection::connect():" << std::endl;
			std::cerr << "\tmyServer: " << myServer.c_str() << std::endl;
			std::cerr << "\tfilename: " << filename << std::endl;
			std::cerr << "\thost:     " << host << std::endl;
			std::cerr << "\tport:     " << port << std::endl;
		}
		myConnection = tcp_connect(host, port);
		if (myConnection == -1) {
			std::cerr << "PrologServer_Connection::connect(): Unable to open socket connection" << std::endl;
			return YUBIMError(4321, "Unable to open socket connection");
		}
		isConnected = 1;
	}
	return mystatus;
}

YUError PrologServer_Connection::disconnect() {
	YUError mystatus;
	if (isConnected) {
		tcp_shutdown(myConnection);
		fclose(myrfile);
		isConnected = 0;
	}
	return mystatus;
}
