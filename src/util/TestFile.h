#ifndef UTIL_TESTFILE_H
#define UTIL_TESTFILE_H

#include <chrono>
#include <thread>

#include <util/File.h>
#include <util/FileUtil.h>

#include <cxxtest/TestSuite.h>

//  #define SLOW_TESTS

/**
 * Test for the File class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestFile : public CxxTest::TestSuite
{
 private:
  void msleep(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }

 public:
  void testBasics()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    File file(filename);
    TS_ASSERT_EQUALS(filename, file.getFilename());

    // Default constructor
    File file3;
    TS_ASSERT(!file3.exists());
  }


  void testExists()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));
    File file(filename);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!file.exists());

    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    TS_ASSERT(FileUtil::exists(filename));
    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }


  void testRead()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    File file(filename);

    // Write some content, check if file exists now
    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    TS_ASSERT(FileUtil::exists(filename));

    // Read the file, check if it has updated since last read.
    const std::string& loaded = file.read();
    TS_ASSERT_EQUALS(loaded, content);
    TS_ASSERT(!file.isUpdated());
    TS_ASSERT(!file.update());
    TS_ASSERT(!file.update());
    TS_ASSERT_EQUALS(file.getLocalCopy(), content);

    // [modify], update, !update, [modify], getLocalCopy, !update
    const std::string text1 {"Text1"};
    FileUtil::write(filename, text1);
    TS_ASSERT(file.update());
    TS_ASSERT(!file.update());
    std::string getText1 = {file.getLocalCopy()};
    TS_ASSERT_EQUALS(text1, getText1);
    TS_ASSERT(!file.update());

    // [modify], update, [modify], read, !update
    const std::string text2 {"Text2"};
    FileUtil::write(filename, text2);
    TS_ASSERT(file.update());
    std::string getText2 = {file.read()};
    TS_ASSERT(!file.update());

#ifdef SLOW_TESTS
    msleep(1200);
    FileUtil::write(filename, "changed");
    TS_ASSERT(file.isUpdated());
    TS_ASSERT(file.update());
#endif

    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }

  void testReadToLocal()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    File file(filename);
    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    file.update();
    TS_ASSERT_EQUALS(file.getLocalCopy(), content);
    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }

  void testCopy()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    // Basic
    File file(filename);
    File filecp = file;
    File fileas(file);
    TS_ASSERT_EQUALS(file.getFilename(), filecp.getFilename());
    TS_ASSERT_EQUALS(file.getFilename(), fileas.getFilename());

    // After copy
    const std::string text1 {"Text1"};
    FileUtil::write(filename, text1);
    TS_ASSERT(file.update());
    File file3(file);
    File file4 = file;
    TS_ASSERT(!file3.update());
    TS_ASSERT(!file4.update());
    TS_ASSERT_EQUALS(file3.read(), text1);

    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }

  void testSetFilename()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string filename("./thisdoesexist.txt");
    FileUtil::write(filename, "foobar");

    File file(filename);
    TS_ASSERT(file.exists());

    const std::string filename2("./certainlythisdoesnotexist.txt");
    file.setFilename(filename2);
    TS_ASSERT(!file.exists());

    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }
};

#endif  // UTIL_TESTFILE_H
