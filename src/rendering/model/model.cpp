//
// Created by Luca Warmenhoven on 21/05/2024.
//

#include <engine/renderer/models/model.h>
#include <fstream>
#include <regex.h>

void split(char *input, char **output, size_t *segment_count,
           const char *delimiter) {
  char *result = strtok(input, delimiter);
  std::vector<char *> results = std::vector<char *>();
  // If no results were found, return.
  if (!result) {
    *output = {};
    *segment_count = 0;
    return;
  }
  results.emplace_back(result);
  while ((result = strtok(nullptr, delimiter))) {
    results.emplace_back(result);
  }
  *segment_count = results.size();
  *output = *results.data();
}

Model *Model::loadObj(const char *filePath) {
  std::ifstream file;
  file.open(filePath);
  if (!file)
    throw std::exception();
  regex_t t;

  std::vector<glm::vec3> vectors = std::vector<glm::vec3>();
  std::vector<glm::vec2> texCoords = std::vector<glm::vec2>();
  std::vector<glm::vec3> normals = std::vector<glm::vec3>();
  std::vector<unsigned int> indices = std::vector<unsigned int>();

  char *segment, *subsegment;
  const char *segment_separator = " ";
  const char *subsegment_separator = "/";

  glm::vec3 vector;

  std::string line;
  while (std::getline(file, line)) {
    // Skip almost-empty lines and comments
    if (line.size() <= 2 || line.starts_with("#"))
      continue;

    segment = strtok(line.data(), " ");

    // Handle vertices
    if (!strcmp(segment, "v")) {
      vectors.emplace_back(glm::vec3(atof(strtok(nullptr, segment_separator)),
                                     atof(strtok(nullptr, segment_separator)),
                                     atof(strtok(nullptr, segment_separator))));

      // Handle indices
    } else if (!strcmp(segment, "f")) {
      subsegment = strtok(segment, "/");

      // Handle Texture coordinates
    } else if (!strcmp(segment, "vt")) {
      vectors.emplace_back(glm::vec3(atof(strtok(nullptr, segment_separator)),
                                     atof(strtok(nullptr, segment_separator)),
                                     atof(strtok(nullptr, segment_separator))));
    }
  }
  file.close();
  return nullptr;
}
