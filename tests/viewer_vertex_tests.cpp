#include "viewer_tests.h"

using namespace s21;

TEST(point, empty_constructor) {
  Point p = Point();
  ASSERT_EQ(p.x_, 0);
  ASSERT_EQ(p.y_, 0);
  ASSERT_EQ(p.z_, 0);
  ASSERT_EQ(p.w_, 1);
}

TEST(point, non_empty_constructor) {
  Point p = Point(1, 2, 3);
  ASSERT_EQ(p.x_, 1);
  ASSERT_EQ(p.y_, 2);
  ASSERT_EQ(p.z_, 3);
  ASSERT_EQ(p.w_, 1);
}

TEST(point, non_empty_constructor_2) {
  Point p = Point(1, 2, 3, 4);
  ASSERT_EQ(p.x_, 1);
  ASSERT_EQ(p.y_, 2);
  ASSERT_EQ(p.z_, 3);
  ASSERT_EQ(p.w_, 4);
  ASSERT_EQ(p.get_x(), 1);
  ASSERT_EQ(p.get_y(), 2);
  ASSERT_EQ(p.get_z(), 3);
  ASSERT_EQ(p.get_w(), 4);
  p.set_x(10);
  p.set_y(20);
  p.set_z(30);
  ASSERT_EQ(p.get_x(), 10);
  ASSERT_EQ(p.get_y(), 20);
  ASSERT_EQ(p.get_z(), 30);
}

TEST(point, equal) {
  Point p = Point(1, 2, 3);
  Point p_2 = Point(1, 2, 3);
  ASSERT_EQ(p == p_2, 1);
}

TEST(vertex, empty_constructor) {
  Vertex v = Vertex();
  ASSERT_EQ(v.get_position().get_x(), 0);
  ASSERT_EQ(v.get_position().get_y(), 0);
  ASSERT_EQ(v.get_position().get_z(), 0);
  ASSERT_EQ(v.get_position().get_w(), 1);
}

TEST(vertex, non_empty_constructor) {
  Point p = Point(1, 2, 3);
  Vertex v = Vertex(p);
  ASSERT_EQ(v.get_position().get_x(), 1);
  ASSERT_EQ(v.get_position().get_y(), 2);
  ASSERT_EQ(v.get_position().get_z(), 3);
  ASSERT_EQ(v.get_position().get_w(), 1);
  Point p_2 = Point(10, 20, 30);
  v.set_position(p_2);
  ASSERT_EQ(v.get_position().get_x(), 10);
  ASSERT_EQ(v.get_position().get_y(), 20);
  ASSERT_EQ(v.get_position().get_z(), 30);
}

TEST(vertex, equal) {
  Point p = Point(1, 2, 3);
  Vertex v = Vertex(p);
  Vertex v_2 = Vertex(p);
  ASSERT_EQ(v == v_2, 1);
}

TEST(point, print_point) {
  Point test_point(1.0, 2.0, 3.0);
  std::ostringstream captured_output;
  std::streambuf* old_cout_buf = std::cout.rdbuf(captured_output.rdbuf());
  test_point.print_point();
  std::cout.rdbuf(old_cout_buf);
  EXPECT_EQ(captured_output.str(), "x = 1 y = 2 z = 3\n");
}

TEST(point, transform) {
  Point p = Point(1, 2, 3);
  Vertex v = Vertex(p);
  float move = 10.0f;
  TransformMatrix move_m =
      TransformMatrixBuilder::create_move_matrix(move, 0, 0);
  v.transform(move_m);
  p = p * move_m;
  ASSERT_EQ(v.get_position() == p, 1);
}