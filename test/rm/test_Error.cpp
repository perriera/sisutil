/**
 * @brief the "MIT/X Consortium License", (adapted for EXPARX.COM)
 *
 * Copyright (C) November 22, 2021 EXPARX INCORPORATED
 *
 * Permission is hereby  granted,  free of charge,  to  any  person
 * obtaining a copy of this software and  associated  documentation
 * files   (the  "Software"),  to deal  in   the  Software  without
 * restriction, including  without  limitation the rights  to  use,
 * copy,  modify, merge,  publish,  distribute,  sublicense, and/or
 * sell copies of the  Software, and to permit persons  to whom the
 * Software  is  furnished to  do  so,  subject  to  the  following
 * conditions:
 *
 * (See LICENSE.md for complete details)
 *
 */

#include <iostream>
#include <sisutil/game/ChessGame.hpp>
#include <sisutil/RMServer.hpp>

#include "../vendor/catch.hpp"

using namespace std;
using namespace util;

class TestRMServer : public RMServer {
public:

    virtual int
        addRM(const std::string& /*optionsString*/,
            const std::string& /*userName*/,
            std::string& /*password*/,
            long /*serverId = 0*/) {
        return 0;
    };
    virtual int reopenAll() const { return 0; };

};

SCENARIO("Verify RMServer", "[RMServer]")
{
    TestRMServer server;
    REQUIRE(true);
}

