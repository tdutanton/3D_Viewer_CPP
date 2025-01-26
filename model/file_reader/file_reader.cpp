#include "file_reader.h"

namespace s21 {

/**
 * @brief Checks if the file was read successfully.
 * @return True if the file was read successfully, otherwise false.
 */
bool FileReader::read_successfull() { return correct_file_; }

/**
 * @brief Extracts vertices from a file line and adds them to the figure.
 * @param current The current line from the file.
 * @param figure Reference to the figure object.
 * @return True if vertices were extracted successfully, otherwise false.
 */
bool FileReader::get_vertices(const std::string& current, Figure& figure) {
  float nums[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  const char* ptr = current.c_str() + 2;
  char* end;
  int fails = 0;
  for (int i = 0; i < 3; i++) {
    nums[i] = strtof(ptr, &end);
    if (ptr == end) fails++;
    ptr = end;
  }
  figure.vertices_.emplace_back(Point(nums[0], nums[1], nums[2], nums[3]));
  set_min_max_for_normalization(figure.vertices_.back());
  return (fails < 2);
}

/**
 * @brief Extracts the figure name from a file line.
 * @param current The current line from the file.
 * @param figure Reference to the figure object.
 * @return True if the name was extracted successfully, otherwise false.
 */
bool FileReader::get_name(const std::string& current, Figure& figure) {
  size_t pos = 2;
  size_t next_pos = current.find_first_of(' ', pos);
  figure.name_ = (next_pos == std::string::npos)
                     ? current.substr(pos)
                     : current.substr(pos, next_pos - pos);
  return !figure.name_.empty();
}

/**
 * @brief Extracts faces from a file line and adds them to the figure.
 * @param current The current line from the file.
 * @param figure Reference to the figure object.
 * @return True if faces were extracted successfully, otherwise false.
 */
bool FileReader::get_faces(const std::string& current, Figure& figure) {
  const char* ptr = current.c_str() + 2;
  std::vector<unsigned> nums;
  nums.reserve(4);
  while (*ptr) {
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    if (!*ptr) break;
    char* end;
    long num = strtol(ptr, &end, 10);
    if (ptr == end || num <= 0 ||
        num > static_cast<int>(figure.vertices_.size())) {
      return false;
    }
    nums.push_back(static_cast<unsigned>(num - 1));
    ptr = end;
    while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
  }
  if (nums.size() < 3) return false;
  figure.faces_.emplace_back(std::move(nums));
  return true;
}

/**
 * @brief Processes a line from the file and updates the figure.
 * @param current The current line from the file.
 * @param figure Reference to the figure object.
 * @return True if the line was processed successfully, otherwise false.
 */
bool FileReader::process_line(const std::string& current, Figure& figure) {
  if (current.empty() || current.length() < 2) return true;
  switch (current[0]) {
    case 'v':
      if (current[1] == ' ') return get_vertices(current, figure);
      break;
    case 'f':
      if (current[1] == ' ') return get_faces(current, figure);
      break;
    case 'o':
      if (current[1] == ' ') return get_name(current, figure);
      break;
  }
  return true;
}

/**
 * @brief Opens the file for reading.
 * @return True if the file was opened successfully, otherwise false.
 * @throws std::runtime_error if the file cannot be opened.
 */
bool FileReader::open_file() {
  file_.open(filename_);
  if (!file_) {
    correct_file_ = false;
    throw std::runtime_error("Cannot open file: " + filename_);
  }
  return true;
}

/**
 * @brief Closes the file if it is open.
 */
void FileReader::close_file() {
  if (file_.is_open()) {
    file_.close();
  }
}

/**
 * @brief Reads figure data from the file and updates the figure object.
 * @param figure Reference to the figure object.
 */
void FileReader::get_data_figure(Figure& figure) {
  open_file();
  std::string current;
  current.reserve(256);
  while (std::getline(file_, current) && correct_file_) {
    if (!current.empty()) {
      correct_file_ = process_line(current, figure);
    }
  }
  if (!figure.vertices_.empty() && correct_file_) {
    normalize_figure(figure);
  } else {
    correct_file_ = false;
  }
  close_file();
}

/**
 * @brief Updates the min and max values for vertex normalization.
 * @param vertex The vertex to process.
 */
void FileReader::set_min_max_for_normalization(Vertex& vertex) {
  NormalizationParameters* params = NormalizationParameters::instance();
  const Point& pos = vertex.get_position();
  const float x = pos.get_x();
  const float y = pos.get_y();
  const float z = pos.get_z();

  params->x_min = std::min(x, params->x_min);
  params->x_max = std::max(x, params->x_max);
  params->y_min = std::min(y, params->y_min);
  params->y_max = std::max(y, params->y_max);
  params->z_min = std::min(z, params->z_min);
  params->z_max = std::max(z, params->z_max);

  const float x_range = std::abs(params->x_max - params->x_min);
  const float y_range = std::abs(params->y_max - params->y_min);
  const float z_range = std::abs(params->z_max - params->z_min);
  params->max_range = std::max(std::max(x_range, y_range), z_range);
}

/**
 * @brief Normalizes a vertex based on the normalization parameters.
 * @param vertex The vertex to normalize.
 */
void FileReader::normalize_vertex(Vertex& vertex) {
  NormalizationParameters* params = NormalizationParameters::instance();
  const float scale = 2.0f / params->max_range;
  const float center_x = (params->x_max + params->x_min) * 0.5f;
  const float center_y = (params->y_max + params->y_min) * 0.5f;
  const float center_z = (params->z_max + params->z_min) * 0.5f;

  const Point& pos = vertex.get_position();
  float x = (pos.get_x() - center_x) * scale;
  float y = (pos.get_y() - center_y) * scale;
  float z = (pos.get_z() - center_z) * scale;

  vertex.set_position(Point(x, y, z));
}

/**
 * @brief Normalizes all vertices in the figure.
 * @param figure Reference to the figure object.
 */
void FileReader::normalize_figure(Figure& figure) {
  for (auto& v : figure.vertices_) {
    normalize_vertex(v);
  }
}

/**
 * @brief Resets the FigureBuilder to its initial state.
 */
void FigureBuilder::reset() {
  params_ = nullptr;
  figure_ = Figure();
}

/**
 * @brief Constructs and returns a figure by reading data from a file.
 * @param filename The name of the file to read.
 * @return The constructed figure.
 */
Figure FigureBuilder::get_result(const std::string& filename) {
  s21::FileReader fileReader;
  fileReader.set_file(filename);
  fileReader.get_data_figure(figure_);
  return figure_;
}

/**
 * @brief Constructs a figure using the provided builder.
 * @param builder The builder to use.
 * @param filename The name of the file to read.
 * @param params Normalization parameters.
 * @return The constructed figure.
 */
Figure FileReaderDirector::construct_figure(FigureBuilder& builder,
                                            const std::string& filename,
                                            NormalizationParameters* params) {
  builder.reset();
  builder.set_normalization_parameters(params);
  return builder.get_result(filename);
}

/**
 * @brief Reads a figure from a file and returns it.
 * @param filename The name of the file to read.
 * @param params Normalization parameters.
 * @return The constructed figure.
 */
Figure FileReader::read_figure(std::string filename,
                               NormalizationParameters* params) {
  FigureBuilder builder;
  FileReaderDirector director;
  return director.construct_figure(builder, filename, params);
}

}  // namespace s21