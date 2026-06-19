#include <gtest/gtest.h>
#include <SharedPtr.hpp>

struct TestObject final {
  static int constructions;
  static int destructions;

  int value;

  TestObject(int v = 0) : value(v) { ++constructions; }

  ~TestObject() { ++destructions; }
};

int TestObject::constructions = 0;
int TestObject::destructions = 0;

class SharedPtrTest : public ::testing::Test {
protected:
  void SetUp() override {
    TestObject::constructions = 0;
    TestObject::destructions = 0;
  }
};

TEST_F(SharedPtrTest, DefaultUseCountIsZero)
{
  not_std::SharedPtr<int> p;
  EXPECT_EQ(p.use_count(), 0);
}

TEST_F(SharedPtrTest, DefaultConstructedIsNull) {
  not_std::SharedPtr<int> ptr;

  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_FALSE(ptr);
}

TEST_F(SharedPtrTest, ConstructFromRawPointer) {
  not_std::SharedPtr<int> ptr(new int(42));

  ASSERT_NE(ptr.get(), nullptr);
  EXPECT_EQ(*ptr, 42);
}


TEST_F(SharedPtrTest, MakeSharedCreatesObject) {
  auto ptr = not_std::MakeShared<TestObject>(123);

  ASSERT_NE(ptr.get(), nullptr);
  EXPECT_EQ(ptr->value, 123);
}

TEST_F(SharedPtrTest, SingleOwnerCountIsOne) {
  auto ptr = not_std::MakeShared<int>(5);

  EXPECT_EQ(ptr.use_count(), 1u);
}

TEST_F(SharedPtrTest, CopyConstructorIncrementsCount) {
  auto p1 = not_std::MakeShared<int>(10);
  auto p2 = p1;

  EXPECT_EQ(p1.use_count(), 2u);
  EXPECT_EQ(p2.use_count(), 2u);
}


TEST_F(SharedPtrTest, CopyAssignmentIncrementsCount) {
  auto p1 = not_std::MakeShared<int>(10);
  not_std::SharedPtr<int> p2;

  p2 = p1;

  EXPECT_EQ(p1.use_count(), 2u);
  EXPECT_EQ(p2.use_count(), 2u);
}

TEST_F(SharedPtrTest, MoveConstructorTransfersOwnership) {
  auto p1 = not_std::MakeShared<int>(7);

  auto p2 = std::move(p1);

  EXPECT_EQ(p1.get(), nullptr);
  EXPECT_EQ(p2.use_count(), 1u);
  EXPECT_EQ(*p2, 7);
}

TEST_F(SharedPtrTest, MoveAssignmentTransfersOwnership) {
  auto p1 = not_std::MakeShared<int>(9);
  not_std::SharedPtr<int> p2;

  p2 = std::move(p1);

  EXPECT_EQ(p1.get(), nullptr);
  EXPECT_EQ(p2.use_count(), 1u);
  EXPECT_EQ(*p2, 9);
}

TEST_F(SharedPtrTest, ResetMakesPointerNull) {
  auto ptr = not_std::MakeShared<int>(1);

  ptr.reset();

  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_FALSE(ptr);
}

TEST_F(SharedPtrTest, ResetDestroysObjectWhenLastOwner) {
  {
    auto ptr = not_std::MakeShared<TestObject>(1);

    EXPECT_EQ(TestObject::destructions, 0);

    ptr.reset();
  }

  EXPECT_EQ(TestObject::destructions, 1);
}

TEST_F(SharedPtrTest, ObjectLivesUntilLastOwner) {
  {
    auto p1 = not_std::MakeShared<TestObject>(1);
    {
      auto p2 = p1;
      p1.reset();

      EXPECT_EQ(TestObject::destructions, 0);
    }
    EXPECT_EQ(TestObject::destructions, 1);
  }
}

TEST_F(SharedPtrTest, DereferenceOperatorWorks) {
  auto ptr = not_std::MakeShared<int>(99);

  EXPECT_EQ(*ptr, 99);
}

TEST_F(SharedPtrTest, ArrowOperatorWorks) {
  auto ptr = not_std::MakeShared<TestObject>(555);

  EXPECT_NE(ptr.get(), nullptr);
  EXPECT_EQ(ptr->value, 555);
}

TEST_F(SharedPtrTest, SelfCopyAssignmentSafe) {
  auto ptr = not_std::MakeShared<int>(12);

  ptr = ptr;

  EXPECT_EQ(ptr.use_count(), 1u);
  EXPECT_EQ(*ptr, 12);
}

TEST_F(SharedPtrTest, SharedOwnersPointToSameObject) {
  auto p1 = not_std::MakeShared<int>(42);
  auto p2 = p1;

  EXPECT_EQ(p1.get(), p2.get());
}

TEST_F(SharedPtrTest, DestructorCalledExactlyOnce) {
  {
    auto p1 = not_std::MakeShared<TestObject>(1);

    {
      auto p2 = p1;
      auto p3 = p2;
      EXPECT_EQ(p1.use_count(), 3);
    }

    EXPECT_EQ(TestObject::destructions, 0);
  }
  EXPECT_EQ(TestObject::destructions, 1);
}
