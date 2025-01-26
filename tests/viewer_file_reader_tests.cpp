#include "viewer_tests.h"

using namespace s21;

TEST(FileReaderTest, ReadSuccessfull) {
  FileReader reader;
  ASSERT_TRUE(reader.read_successfull());

  reader.set_file("tests/cube.obj");
  try {
    reader.open_file();
  } catch (const std::runtime_error& e) {
    FAIL() << "Unexpected error: " << e.what();
  }
  ASSERT_TRUE(reader.read_successfull());
}

TEST(FileReaderTest, GetVertices) {
  FileReader reader;
  Figure figure;

  std::string vertex_line = "v 1.0 2.0 3.0";
  ASSERT_TRUE(reader.get_vertices(vertex_line, figure));
  ASSERT_EQ(figure.get_vertices().size(), 1);

  const auto& vertex = figure.get_vertices()[0].get_position();
  ASSERT_FLOAT_EQ(vertex.get_x(), 1.0f);
  ASSERT_FLOAT_EQ(vertex.get_y(), 2.0f);
  ASSERT_FLOAT_EQ(vertex.get_z(), 3.0f);
}

TEST(FileReaderTest, GetName) {
  FileReader reader;
  Figure figure;

  std::string name_line = "o TestObject";
  ASSERT_TRUE(reader.get_name(name_line, figure));
  ASSERT_EQ(figure.get_name(), "TestObject");
}

TEST(FileReaderTest, GetFaces) {
  FileReader reader;
  Figure figure;

  Vertex v1, v2, v3;
  figure.vertices_.push_back(v1);
  figure.vertices_.push_back(v2);
  figure.vertices_.push_back(v3);

  std::string face_line = "f 1 2 3";
  ASSERT_TRUE(reader.get_faces(face_line, figure));
  ASSERT_EQ(figure.get_faces().size(), 1);
  ASSERT_EQ(figure.get_faces()[0].get_face().size(), 3);
}

TEST(FileReaderTest, ProcessLine) {
  FileReader reader;
  Figure figure;

  std::string vertex_line = "v 1.0 2.0 3.0";
  ASSERT_TRUE(reader.process_line(vertex_line, figure));

  std::string name_line = "o TestObject";
  ASSERT_TRUE(reader.process_line(name_line, figure));

  std::string face_line = "f 1 2 3";
  ASSERT_FALSE(reader.process_line(face_line, figure));

  ASSERT_EQ(figure.get_vertices().size(), 1);
  ASSERT_EQ(figure.get_name(), "TestObject");
}

TEST(FileReaderTest, OpenFile) {
  FileReader reader;
  reader.set_file("nonexistent_file.obj");
  ASSERT_THROW(reader.open_file(), std::runtime_error);
}

TEST(FileReaderTest, GetDataFigure) {
  FileReader reader;
  Figure figure;
  reader.set_file("tests/cube.obj");

  try {
    reader.get_data_figure(figure);
  } catch (const std::exception& e) {
    FAIL() << "Unexpected error: " << e.what();
  }

  ASSERT_TRUE(reader.read_successfull());
  ASSERT_FALSE(figure.get_vertices().empty());
}

TEST(FileReaderTest, BuilderTestFigure) {
  NormalizationParameters* params;
  params = NormalizationParameters::instance();
  FileReader reader;
  Figure figure;
  figure = reader.read_figure("tests/cube.obj", params);
  figure.make_edges();
  EXPECT_EQ(figure.vertices_.size(), 8);
  EXPECT_EQ(figure.edges_.size(), 12);
  EXPECT_EQ(figure.all_edges_count_, 24);
}

void create_temp_file(const std::string& filename, const std::string& content) {
  std::ofstream file(filename);
  file << content;
  file.close();
}

TEST(FileReaderTest, GetDataFigureEmptyFileTest) {
  std::string filename = "empty_figure.obj";
  create_temp_file(filename, "");
  FileReader reader;
  reader.set_file(filename);
  Figure figure;
  reader.get_data_figure(figure);
  EXPECT_TRUE(figure.vertices_.empty());
  EXPECT_TRUE(figure.faces_.empty());
  std::remove(filename.c_str());
}

TEST(FileReaderTest, GetDataFigureInvalidFileTest) {
  std::string filename = "invalid_figure.obj";
  std::string file_content =
      "v 1.0 2.0 3.0\n"
      "f 1 2\n";
  create_temp_file(filename, file_content);
  FileReader reader;
  reader.set_file(filename);
  Figure figure;
  reader.get_data_figure(figure);
  EXPECT_FALSE(reader.correct_file_);
  std::remove(filename.c_str());
}

TEST(FileReaderTest, CloseFile) {
  FileReader reader;
  reader.set_file("tests/cube.obj");
  try {
    reader.open_file();
  } catch (const std::runtime_error& e) {
    FAIL() << "Unexpected error: " << e.what();
  }
  reader.close_file();
  ASSERT_FALSE(reader.file_.is_open());
}

TEST(FileReaderTest, CloseFile_2) {
  FileReader reader;
  reader.set_file("tests/cube.obj");

  ASSERT_NO_THROW(reader.open_file());
  reader.close_file();
  ASSERT_FALSE(reader.file_.is_open());
}

TEST(FileReaderTest, NormalizeFigure) {
  FileReader reader;
  Figure figure;
  Vertex v1(Point(1.0, 2.0, 3.0));
  Vertex v2(Point(4.0, 5.0, 6.0));
  figure.vertices_.push_back(v1);
  figure.vertices_.push_back(v2);
  reader.normalize_figure(figure);
  const auto& normalized_v1 = figure.vertices_[0].get_position();
  EXPECT_FLOAT_EQ(normalized_v1.get_x(), 0.42857146);
  EXPECT_FLOAT_EQ(normalized_v1.get_y(), 0.71428573f);
  EXPECT_FLOAT_EQ(normalized_v1.get_z(), 1.0f);
}
