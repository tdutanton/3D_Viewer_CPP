#ifndef FILE_READER_H_
#define FILE_READER_H_

#include <fstream>
#include <string>

#include "../facade/normalization_parameters.h"
#include "../figure/figure.h"

namespace s21 {

class Figure;
class NormalizationParameters;

/**
 * @class FileReader
 * @brief A class for reading figure data from a file.
 *
 * This class provides functionality for reading figure data from a file,
 * processing the data, and normalizing vertex coordinates.
 * @note In test mode (`#ifdef TEST`), the member variables are public for
 * testing purposes.
 */
class FileReader {
 public:
  FileReader() = default;
  ~FileReader() = default;

#ifdef TEST
 public:
#else
 private:
#endif
  std::string filename_;      ///< The name of the file to read.
  std::ifstream file_;        ///< File stream for reading.
  bool correct_file_ = true;  ///< Flag indicating whether the file is valid.

 public:
  Figure read_figure(std::string filename, NormalizationParameters* params);

  /**
   * @brief Sets the file name.
   * @param filename The name of the file.
   */
  void set_file(std::string filename) { filename_ = filename; };
  void get_data_figure(Figure& figure);
  bool process_line(const std::string& current, Figure& figure);
  bool open_file();
  void close_file();
  bool read_successfull();
  bool get_vertices(const std::string& current, Figure& figure);
  bool get_faces(const std::string& current, Figure& figure);
  bool get_name(const std::string& current, Figure& figure);
  void set_min_max_for_normalization(Vertex& vertex);
  void normalize_vertex(Vertex& vertex);
  void normalize_figure(Figure& figure);
};

/**
 * @class FileReaderBuilder
 * @brief Abstract base class for building a FileReader. Use Builder pattern.
 *
 * This class defines the interface for building a FileReader object.
 */
class FileReaderBuilder {
 public:
  /**
   * @brief Resets the builder.
   */
  virtual void reset() = 0;

  /**
   * @brief Sets normalization parameters.
   * @param params Normalization parameters.
   */
  virtual void set_normalization_parameters(
      NormalizationParameters* params) = 0;
  virtual ~FileReaderBuilder() = default;
};

/**
 * @class FigureBuilder
 * @brief A concrete builder for creating Figure objects.
 *
 * This class implements the FileReaderBuilder interface to construct Figure
 * objects.
 */
class FigureBuilder : public FileReaderBuilder {
 private:
  NormalizationParameters* params_;  ///< Normalization parameters.
  Figure figure_;                    ///< The figure being built.

 public:
  void reset() override;

  /**
   * @brief Sets normalization parameters.
   * @param params Normalization parameters.
   */
  void set_normalization_parameters(NormalizationParameters* params) override {
    params_ = params;
  }
  Figure get_result(const std::string& filename);
};

/**
 * @brief Constructs a figure using the provided builder.
 * @param builder The builder to use.
 * @param filename The name of the file to read.
 * @param params Normalization parameters.
 * @return The constructed figure.
 */
class FileReaderDirector {
 public:
  Figure construct_figure(FigureBuilder& builder, const std::string& filename,
                          NormalizationParameters* params);
};

}  // namespace s21

#endif