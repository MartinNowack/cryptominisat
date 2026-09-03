/******************************************
Copyright (C) 2009-2020 Authors of CryptoMiniSat, see AUTHORS file

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
***********************************************/

#include "gtest/gtest.h"
#include "cryptominisat5/cryptominisat.h"
#include "test_helper.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <string>

using namespace CMSat;
using std::vector;
using std::string;

// Test that set_prefix with default "c " prefix doesn't crash
TEST(set_prefix_test, default_prefix_no_crash)
{
    SATSolver s;
    s.new_vars(3);
    s.add_clause(str_to_cl("1"));
    // Set default prefix - should not crash
    s.set_prefix("c ");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix with a custom prefix doesn't crash
TEST(set_prefix_test, custom_prefix_no_crash)
{
    SATSolver s;
    s.new_vars(3);
    s.add_clause(str_to_cl("1, 2"));
    // Set a custom prefix - should not crash
    s.set_prefix("MY_PREFIX ");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix with an empty string doesn't crash
TEST(set_prefix_test, empty_prefix_no_crash)
{
    SATSolver s;
    s.new_vars(3);
    s.add_clause(str_to_cl("1, 2, 3"));
    // Set empty prefix - should not crash
    s.set_prefix("");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix can be called multiple times
TEST(set_prefix_test, set_multiple_times)
{
    SATSolver s;
    s.new_vars(3);
    s.add_clause(str_to_cl("1"));
    // Set prefix multiple times - should not crash
    s.set_prefix("c ");
    s.set_prefix("PREFIX1 ");
    s.set_prefix("PREFIX2 ");
    s.set_prefix("c ");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix works before adding variables and clauses
TEST(set_prefix_test, set_prefix_before_vars)
{
    SATSolver s;
    // Set prefix before adding variables - should not crash
    s.set_prefix("EARLY ");
    s.new_vars(2);
    s.add_clause(str_to_cl("1, 2"));
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix works with multi-threaded solver
TEST(set_prefix_test, multi_thread_prefix)
{
    SATSolver s;
    s.set_num_threads(2);
    s.new_vars(4);
    s.add_clause(str_to_cl("1, 2"));
    s.add_clause(str_to_cl("3, 4"));
    // Set prefix in multi-threaded mode - should not crash
    s.set_prefix("MT_PREFIX ");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix changes the print_stats output prefix
// We capture stdout and verify the prefix appears in output
TEST(set_prefix_test, custom_prefix_in_stats_output)
{
    SATSolver s;
    s.new_vars(3);
    s.add_clause(str_to_cl("1"));
    s.set_prefix("TESTPFX ");

    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);

    // Capture stdout to check prefix in output
    std::ostringstream captured;
    std::streambuf* old_cout = std::cout.rdbuf(captured.rdbuf());
    s.print_stats();
    std::cout.rdbuf(old_cout);

    string output = captured.str();
    // Verify that the custom prefix appears in the statistics output
    EXPECT_NE(output.find("TESTPFX"), string::npos);
}

// Test that set_prefix with a long string works
TEST(set_prefix_test, long_prefix_no_crash)
{
    SATSolver s;
    s.new_vars(2);
    s.add_clause(str_to_cl("1, 2"));
    // Use a longer prefix
    s.set_prefix("LONG_PREFIX_STRING_THAT_IS_QUITE_LONG_INDEED ");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test that set_prefix works after a solve call
TEST(set_prefix_test, set_prefix_after_solve)
{
    SATSolver s;
    s.new_vars(3);
    s.add_clause(str_to_cl("1, 2"));
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_True);

    // Set prefix after solve - should not crash
    s.set_prefix("AFTER_SOLVE ");
    s.add_clause(str_to_cl("3"));
    ret = s.solve();
    EXPECT_EQ(ret, l_True);
}

// Test set_prefix with UNSAT instance
TEST(set_prefix_test, prefix_with_unsat)
{
    SATSolver s;
    s.new_vars(2);
    s.add_clause(str_to_cl("1"));
    s.add_clause(str_to_cl("-1"));
    s.set_prefix("UNSAT_TEST ");
    lbool ret = s.solve();
    EXPECT_EQ(ret, l_False);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
