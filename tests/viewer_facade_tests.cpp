#include "viewer_tests.h"

using namespace s21;

namespace s21 {
class MockSceneDrawer : public SceneDrawerBase {
 public:
  void DrawFigure(Figure* figure [[maybe_unused]]) override {}
};

class FacadeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    facade = new Facade();
    facade->sceneDrawer_ = new MockSceneDrawer();
    params = NormalizationParameters::instance();
  }

  void TearDown() override {
    if (facade) {
      if (facade->figure_) {
        delete facade->figure_;
        facade->figure_ = nullptr;
      }
      if (facade->sceneDrawer_) {
        delete facade->sceneDrawer_;
        facade->sceneDrawer_ = nullptr;
      }
      if (facade->fileReader_) {
        delete facade->fileReader_;
        facade->fileReader_ = nullptr;
      }
      delete facade;
    }
    params->reset_normalization_parameters();
  }

  Facade* facade;
  NormalizationParameters* params;
};

}  // namespace s21

TEST(FacadeTest_simple, ResetNormalize) {
  NormalizationParameters* params = NormalizationParameters::instance();
  params->reset_normalization_parameters();
  EXPECT_EQ(params->x_min, 0);
  EXPECT_EQ(params->x_max, 0);
  EXPECT_EQ(params->dx_step, 0.01f);
  EXPECT_EQ(params->y_min, 0);
  EXPECT_EQ(params->y_max, 0);
  EXPECT_EQ(params->dy_step, 0.01f);
  EXPECT_EQ(params->z_min, 0);
  EXPECT_EQ(params->z_max, 0);
  EXPECT_EQ(params->dz_step, 0.01f);
  EXPECT_EQ(params->max_range, 0);
}

TEST_F(FacadeTest, LoadValidFile) {
  auto result = facade->load_figure("tests/cube.obj");
  EXPECT_TRUE(result.IsSuccess());
  EXPECT_EQ(facade->get_vertices_count(), 8);
  EXPECT_EQ(facade->get_unique_edges_count(), 12);
  EXPECT_EQ(facade->get_all_edges_count(), 24);
}

TEST_F(FacadeTest, TransformOperations) {
  facade->load_figure("tests/cube.obj");
  auto move_result = facade->move_figure(1.0f, 2.0f, 3.0f);
  EXPECT_TRUE(move_result.IsSuccess());
  auto rotate_result = facade->rotate_figure(45.0f, 0.0f, 0.0f);
  EXPECT_TRUE(rotate_result.IsSuccess());
  auto scale_result = facade->scale_figure(2.0f, 2.0f, 2.0f);
  EXPECT_TRUE(scale_result.IsSuccess());
  facade->reset_figure_pos();
  EXPECT_EQ(facade->get_vertices_count(), 8);
}

TEST(FacadeOperationResultTest, SetErrorMessage) {
  FacadeOperationResult res;
  std::string error_message = "Test error message";
  res.SetErrorMessage(error_message);
  EXPECT_EQ(res.GetErrorMessage(), error_message);
}

TEST(FacadeOperationResultTest, SetSuccess) {
  FacadeOperationResult res;
  res.SetSuccess(false);
  EXPECT_FALSE(res.IsSuccess());
}

TEST_F(FacadeTest, LoadFigureFileReadingError) {
  std::string invalidPath = "invalid_path.obj";
  s21::FacadeOperationResult result = facade->load_figure(invalidPath);
  EXPECT_FALSE(result.IsSuccess());
  EXPECT_EQ(result.GetErrorMessage(), "File reading error");
}

TEST_F(FacadeTest, LoadFigureNullPointers) {
  delete facade->fileReader_;
  delete facade->figure_;
  facade->fileReader_ = nullptr;
  facade->figure_ = nullptr;
  s21::FacadeOperationResult result = facade->load_figure("invalid_path.obj");
  EXPECT_FALSE(result.IsSuccess());
  EXPECT_EQ(result.GetErrorMessage(),
            "Internal error: fileReader or figure is null.");
}

TEST_F(FacadeTest, LoadFigureSuccess) {
  std::string validPath = "tests/cube.obj";
  s21::FacadeOperationResult result = facade->load_figure(validPath);
  EXPECT_TRUE(result.IsSuccess());
}

TEST_F(FacadeTest, LoadFigureFileReadingError_2) {
  std::string invalidPath = "invalid_path.obj";
  s21::FacadeOperationResult result = facade->load_figure(invalidPath);
  EXPECT_FALSE(result.IsSuccess());
  EXPECT_EQ(result.GetErrorMessage(), "File reading error");
}

TEST_F(FacadeTest, LoadFigureStdException) {
  std::string exceptionPath = "exception_path.obj";
  s21::FacadeOperationResult result = facade->load_figure(exceptionPath);
  EXPECT_FALSE(result.IsSuccess());
  EXPECT_EQ(result.GetErrorMessage(), "File reading error");
}

TEST_F(FacadeTest, LoadFigureUnknownException) {
  std::string unknownExceptionPath = "unknown_exception_path.obj";
  s21::FacadeOperationResult result = facade->load_figure(unknownExceptionPath);
  EXPECT_FALSE(result.IsSuccess());
  EXPECT_EQ(result.GetErrorMessage(), "File reading error");
}