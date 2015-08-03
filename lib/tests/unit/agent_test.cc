#include "certs.hpp"

#include "root_path.hpp"

#include <cthun-agent/agent.hpp>
#include <cthun-agent/agent_configuration.hpp>

#include <catch.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

namespace CthunAgent {

const std::string TEST_SERVER_URL { "wss://127.0.0.1:8090/cthun/" };
static const std::string MODULES { std::string { CTHUN_AGENT_ROOT_PATH }
                                   + "/lib/tests/resources/modules" };
const std::string SPOOL { std::string { CTHUN_AGENT_ROOT_PATH }
                          + "/lib/tests/resources/tmp/" };

TEST_CASE("Agent::Agent", "[agent]") {
    AgentConfiguration agent_configuration { MODULES,
                                             TEST_SERVER_URL,
                                             getCaPath(),
                                             getCertPath(),
                                             getKeyPath(),
                                             SPOOL,
                                             "",  // modules config dir
                                             "test_agent" };

    SECTION("does not throw if it fails to find the external modules directory") {
        agent_configuration.modules_dir = MODULES + "/fake_dir";

        REQUIRE_NOTHROW(Agent { agent_configuration });
    }

    SECTION("should throw an Agent::Error if client cert path is invalid") {
        agent_configuration.crt = "spam";

        REQUIRE_THROWS_AS(Agent { agent_configuration }, Agent::Error);
    }

    SECTION("successfully instantiates with valid arguments") {
        REQUIRE_NOTHROW(Agent { agent_configuration });
    }

    boost::filesystem::remove_all(SPOOL);
}

}  // namespace CthunAgent
