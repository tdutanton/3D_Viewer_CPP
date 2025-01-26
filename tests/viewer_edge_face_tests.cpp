#include "viewer_tests.h"

using namespace s21;

TEST(edge, common) {
  Point p_1(1, 2, 3);
  Point p_2(4, 5, 6);
  Vertex v_1(p_1);
  Vertex v_2(p_2);
  Edge e(v_1, v_2);

  EXPECT_FLOAT_EQ(e.get_begin().get_position().get_x(), 1);
  EXPECT_FLOAT_EQ(e.get_begin().get_position().get_y(), 2);
  EXPECT_FLOAT_EQ(e.get_begin().get_position().get_z(), 3);
  EXPECT_FLOAT_EQ(e.get_end().get_position().get_x(), 4);
  EXPECT_FLOAT_EQ(e.get_end().get_position().get_y(), 5);
  EXPECT_FLOAT_EQ(e.get_end().get_position().get_z(), 6);

  Point p_3(10, 20, 30);
  Point p_4(40, 50, 60);
  Vertex v_3(p_3);
  Vertex v_4(p_4);
  Edge e_2(v_3, v_4);
  e_2 = e;
  EXPECT_FLOAT_EQ(e_2.get_begin().get_position().get_x(), 1);
  EXPECT_FLOAT_EQ(e_2.get_begin().get_position().get_y(), 2);
  EXPECT_FLOAT_EQ(e_2.get_begin().get_position().get_z(), 3);
  EXPECT_FLOAT_EQ(e_2.get_end().get_position().get_x(), 4);
  EXPECT_FLOAT_EQ(e_2.get_end().get_position().get_y(), 5);
  EXPECT_FLOAT_EQ(e_2.get_end().get_position().get_z(), 6);

  ASSERT_EQ(e_2 == e, 1);
}

TEST(face, common) {
  Face f;
  EXPECT_TRUE(f.get_face().empty());

  std::vector<unsigned> vertices{1, 2, 3, 4};
  Face f_n(vertices);
  EXPECT_EQ(f_n.get_face(), vertices);

  std::vector<unsigned> vertices1{1, 2, 3};
  std::vector<unsigned> vertices2{1, 2, 3};
  std::vector<unsigned> vertices3{4, 5, 6};
  Face f1(vertices1);
  Face f2(vertices2);
  Face f3(vertices3);
  ASSERT_EQ(f1 == f2, 1);
  ASSERT_EQ(f1 == f3, 0);

  std::vector<unsigned> vertices_2{5, 6, 7, 8};
  Face f_v(vertices_2);
  EXPECT_EQ(f_v.get_face().size(), 4);
  EXPECT_EQ(f_v.get_face(), vertices_2);
}