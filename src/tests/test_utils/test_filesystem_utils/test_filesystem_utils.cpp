#include "gtest/gtest.h"
#include "common/utils/filesystem_utils.h"

using namespace common::utils::filesystem;

namespace test::test_utils::test_filesystem_utils {

TEST(Process, case) {
    auto path = GetProcessPath();
    EXPECT_FALSE(path.empty());
    EXPECT_TRUE(IsRegularFile(path));
}
}  // namespace test_utils::test_filesystem_utils
