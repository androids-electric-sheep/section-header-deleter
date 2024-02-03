#include <cstring>
#include <elf.h>
#include <fstream>
#include <iostream>
#include <iterator>

std::string get_file_contents(const std::string &filename) {
  std::ifstream ifs{filename, std::ios::in | std::ios::binary};
  std::string str(std::istreambuf_iterator<char>{ifs}, {});
  return str;
}

void overwrite_section_headers(std::string &file_contents) {
  Elf64_Ehdr *ehdr = reinterpret_cast<Elf64_Ehdr *>(&file_contents[0]);
  Elf64_Shdr *shdr =
      reinterpret_cast<Elf64_Shdr *>(&file_contents[ehdr->e_shoff]);
  // Zero out the memory of each section header
  for (int i = 0; i < ehdr->e_shnum; ++i) {
    std::cout << "[+] Overwriting section header " << i << std::endl;
    std::memset(shdr, 0, ehdr->e_shentsize);
  }
  // Overwrite section header references in the file header
  ehdr->e_shoff = 0;
  ehdr->e_shnum = 0;
  ehdr->e_shstrndx = 0;
  ehdr->e_shentsize = 0;
}

void write_file(const std::string &filename, const std::string &file_contents) {
  std::ofstream ofs{filename, std::ios::out | std::ios::binary};
  ofs << file_contents;
  ofs.close();
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <INPUT_FILE> <OUTPUT_FILE>"
              << std::endl;
    return EXIT_FAILURE;
  }

  const std::string input_filename{argv[1]};
  const std::string output_filename{argv[2]};

  std::string file_contents = get_file_contents(input_filename);
  std::cout << "[+] Read file contents, " << file_contents.length() << " bytes"
            << std::endl;

  overwrite_section_headers(file_contents);
  write_file(output_filename, file_contents);

  return EXIT_SUCCESS;
}
