#include "viewer_tests.h"

using namespace s21;

TEST(matrix, deg_to_rad) {
  EXPECT_FLOAT_EQ(deg_to_rad(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(deg_to_rad(90.0f), M_PI / 2);
  EXPECT_FLOAT_EQ(deg_to_rad(180.0f), M_PI);
  EXPECT_FLOAT_EQ(deg_to_rad(360.0f), 2 * M_PI);
  EXPECT_FLOAT_EQ(deg_to_rad(-90.0f), -M_PI / 2);
  EXPECT_NEAR(deg_to_rad(45.0f), M_PI / 4, 1e-5);
}

TEST(matrix, constructor_and_reset) {
  TransformMatrix m = TransformMatrix();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(m.get_elem(i, j), (i == j) ? 1 : 0);
    }
  }
  m.reset();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(m.get_elem(i, j), 0);
    }
  }
}

TEST(matrix, set_elem) {
  TransformMatrix m = TransformMatrix();
  float f = 3.14;
  m.set_elem(0, 0, f);
  EXPECT_FLOAT_EQ(m.get_elem(0, 0), f);
  EXPECT_ANY_THROW(m.set_elem(5, 5, f));
  EXPECT_ANY_THROW(m.set_elem(-5, -5, f));
}

TEST(matrix, operator_mult) {
  TransformMatrix m = TransformMatrix();
  TransformMatrix m_2 = TransformMatrix();
  m.set_elem(0, 0, 2);
  m_2.set_elem(0, 0, 4);
  m = m * m_2;
  EXPECT_FLOAT_EQ(m.get_elem(0, 0), 8);
}

TEST(matrix, point_multiplication) {
  TransformMatrix m;

  Point p1(1.0f, 2.0f, 3.0f, 1.0f);
  Point result1 = m * p1;
  EXPECT_FLOAT_EQ(result1.x_, 1.0f);
  EXPECT_FLOAT_EQ(result1.y_, 2.0f);
  EXPECT_FLOAT_EQ(result1.z_, 3.0f);
  EXPECT_FLOAT_EQ(result1.w_, 1.0f);

  TransformMatrix m2;
  m2.set_elem(0, 0, 2.0f);
  m2.set_elem(1, 1, 3.0f);
  m2.set_elem(2, 2, 4.0f);
  Point p2(1.0f, 1.0f, 1.0f, 1.0f);
  Point result2 = m2 * p2;
  EXPECT_FLOAT_EQ(result2.x_, 2.0f);
  EXPECT_FLOAT_EQ(result2.y_, 3.0f);
  EXPECT_FLOAT_EQ(result2.z_, 4.0f);
  EXPECT_FLOAT_EQ(result2.w_, 1.0f);

  TransformMatrix m3;
  m3.set_elem(0, 3, 5.0f);
  m3.set_elem(1, 3, 6.0f);
  m3.set_elem(2, 3, 7.0f);
  Point p3(0.0f, 0.0f, 0.0f, 1.0f);
  Point result3 = m3 * p3;
  EXPECT_FLOAT_EQ(result3.x_, 5.0f);
  EXPECT_FLOAT_EQ(result3.y_, 6.0f);
  EXPECT_FLOAT_EQ(result3.z_, 7.0f);
  EXPECT_FLOAT_EQ(result3.w_, 1.0f);

  Point p4(0.0f, 0.0f, 0.0f, 0.0f);
  Point result4 = m * p4;
  EXPECT_FLOAT_EQ(result4.x_, 0.0f);
  EXPECT_FLOAT_EQ(result4.y_, 0.0f);
  EXPECT_FLOAT_EQ(result4.z_, 0.0f);
  EXPECT_FLOAT_EQ(result4.w_, 0.0f);
}

TEST(matrix, transform_point) {
  TransformMatrix m;

  Point p1(1.0f, 2.0f, 3.0f, 1.0f);
  Point result1 = m.transform_point(p1);
  EXPECT_FLOAT_EQ(result1.x_, 1.0f);
  EXPECT_FLOAT_EQ(result1.y_, 2.0f);
  EXPECT_FLOAT_EQ(result1.z_, 3.0f);
  EXPECT_FLOAT_EQ(result1.w_, 1.0f);

  TransformMatrix scale_matrix;
  scale_matrix.set_elem(0, 0, 2.0f);
  scale_matrix.set_elem(1, 1, 3.0f);
  scale_matrix.set_elem(2, 2, 4.0f);
  Point p2(1.0f, 1.0f, 1.0f, 1.0f);
  Point result2 = scale_matrix.transform_point(p2);
  EXPECT_FLOAT_EQ(result2.x_, 2.0f);
  EXPECT_FLOAT_EQ(result2.y_, 3.0f);
  EXPECT_FLOAT_EQ(result2.z_, 4.0f);
  EXPECT_FLOAT_EQ(result2.w_, 1.0f);

  TransformMatrix translation_matrix;
  translation_matrix.set_elem(0, 3, 5.0f);
  translation_matrix.set_elem(1, 3, 6.0f);
  translation_matrix.set_elem(2, 3, 7.0f);
  Point p3(0.0f, 0.0f, 0.0f, 1.0f);
  Point result3 = translation_matrix.transform_point(p3);
  EXPECT_FLOAT_EQ(result3.x_, 5.0f);
  EXPECT_FLOAT_EQ(result3.y_, 6.0f);
  EXPECT_FLOAT_EQ(result3.z_, 7.0f);
  EXPECT_FLOAT_EQ(result3.w_, 1.0f);
}

TEST(matrix, matrix_builder) {
  float angle = 90;
  TransformMatrix x_rot =
      TransformMatrixBuilder::create_only_x_rotation_matrix(angle);
  TransformMatrix m_check;
  m_check.set_elem(1, 1, cos(angle));
  m_check.set_elem(1, 2, -sin(angle));
  m_check.set_elem(2, 1, sin(angle));
  m_check.set_elem(2, 2, cos(angle));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(x_rot.get_elem(i, j), m_check.get_elem(i, j));
    }
  }
  TransformMatrix y_rot =
      TransformMatrixBuilder::create_only_y_rotation_matrix(angle);
  TransformMatrix m_check_2;
  m_check_2.set_elem(0, 0, cos(-angle));
  m_check_2.set_elem(0, 2, -sin(-angle));
  m_check_2.set_elem(2, 0, sin(-angle));
  m_check_2.set_elem(2, 2, cos(-angle));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(y_rot.get_elem(i, j), m_check_2.get_elem(i, j));
    }
  }
  TransformMatrix z_rot =
      TransformMatrixBuilder::create_only_z_rotation_matrix(angle);
  TransformMatrix m_check_3;
  m_check_3.set_elem(0, 0, cos(angle));
  m_check_3.set_elem(0, 1, -sin(angle));
  m_check_3.set_elem(1, 0, sin(angle));
  m_check_3.set_elem(1, 1, cos(angle));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(z_rot.get_elem(i, j), m_check_3.get_elem(i, j));
    }
  }
  TransformMatrix rot =
      TransformMatrixBuilder::create_rotation_matrix(angle, angle, angle);
  m_check =
      TransformMatrixBuilder::create_only_x_rotation_matrix(deg_to_rad(angle));
  m_check_2 =
      TransformMatrixBuilder::create_only_y_rotation_matrix(deg_to_rad(angle));
  m_check_3 =
      TransformMatrixBuilder::create_only_z_rotation_matrix(deg_to_rad(angle));
  TransformMatrix check_rot = m_check_3 * m_check_2 * m_check;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(rot.get_elem(i, j), check_rot.get_elem(i, j));
    }
  }

  float x = 10.0f;
  float y = 20.0f;
  float z = 30.0f;
  TransformMatrix move = TransformMatrixBuilder::create_move_matrix(x, y, z);
  TransformMatrix move_check;
  move_check.set_elem(0, 3, x);
  move_check.set_elem(1, 3, y);
  move_check.set_elem(2, 3, z);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(move.get_elem(i, j), move_check.get_elem(i, j));
    }
  }

  TransformMatrix scale = TransformMatrixBuilder::create_scale_matrix(x, y, z);
  TransformMatrix scale_check;
  scale_check.set_elem(0, 0, x);
  scale_check.set_elem(1, 1, y);
  scale_check.set_elem(2, 2, z);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(scale.get_elem(i, j), scale_check.get_elem(i, j));
    }
  }
}
