#include "viewer_tests.h"

using namespace s21;

namespace s21 {
class FigureTest : public ::testing::Test {
 public:
  Figure figure_;
  void SetUp() override {
    figure_.set_name("Test Figure");
    Point p_1(0, 0, 0);
    Point p_2(1, 0, 0);
    Point p_3(1, 1, 0);
    Point p_4(0, 1, 0);
    figure_.vertices_.emplace_back(Vertex(p_1));
    figure_.vertices_.emplace_back(Vertex(p_2));
    figure_.vertices_.emplace_back(Vertex(p_3));
    figure_.vertices_.emplace_back(Vertex(p_4));
    std::vector<unsigned> v = {0, 1, 2, 3};
    Face face(v);
    figure_.faces_.push_back(face);
    figure_.faces_.push_back(face);
    figure_.save_default_coords();
    figure_.make_edges();
  }
};
}  // namespace s21

TEST_F(FigureTest, SetAndGetName) {
  EXPECT_EQ(figure_.get_name(), "Test Figure");
  std::string name_n = "New name";
  figure_.set_name(name_n);
  EXPECT_EQ(figure_.get_name() == "New name", 1);
}

TEST_F(FigureTest, TransformVertices) {
  TransformMatrix matrix = TransformMatrixBuilder::create_scale_matrix(2, 2, 2);
  figure_.transform(matrix);
  EXPECT_EQ(figure_.get_vertices()[0].get_position(), Point(0, 0, 0));
  EXPECT_EQ(figure_.get_vertices()[1].get_position(), Point(2, 0, 0));
  EXPECT_EQ(figure_.get_vertices()[2].get_position(), Point(2, 2, 0));
  EXPECT_EQ(figure_.get_vertices()[3].get_position(), Point(0, 2, 0));
  figure_.get_default_coords();
  EXPECT_EQ(figure_.vertices_, figure_.default_vertices_);
}

TEST_F(FigureTest, MakeEdges) {
  EXPECT_EQ(figure_.get_edges().size(), 4);
  EXPECT_TRUE(figure_.get_all_edges_count() == 8);
}