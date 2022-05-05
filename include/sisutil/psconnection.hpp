// PrologServer_Connection.hpp - IPC class for AD rule processing engine
// $Id: psconnection.hpp,v 1.1 2000/09/28 15:54:01 ed Exp $

#ifndef PrologServer_Connection_HPP
#define PrologServer_Connection_HPP
#include <stdio.h>
#include <string>
#include <sisutil/YUError.hpp>

class PrologServer_Connection {
public:
  inline PrologServer_Connection(const std::string& server)
    : isConnected(0), myConnection(0), myrfile(NULL), myServer(server) {};
  inline virtual ~PrologServer_Connection() { disconnect(); };
  YUError send(const char*);
  //YUError transceive(const char*, char *ret, long size);
  YUError transceive(const std::string&, char* ret, long size);

protected:
  YUError connect();
  YUError disconnect();
  int     isConnected;
  int     myConnection;
  FILE* myrfile;

private:
  // disable copy ctor
  PrologServer_Connection(const PrologServer_Connection&);
  // disable op=
  PrologServer_Connection& operator=(const PrologServer_Connection&);
  std::string myServer;
};  // PrologServer_Connection

// Name: PrologServer_Connection
// Documentation: IPC class for Prolog rule processing engine
// Visibility: exported | private | imported
// Cardinality: 1
// Concurrency: blocking
// Persistance: dynamic
#endif
