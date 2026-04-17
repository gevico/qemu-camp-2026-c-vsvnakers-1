#include "myfile.h"

static const char *resolve_path(const char *input, char *buf, size_t buf_size) {
    if (access(input, F_OK) == 0) {
      return input;
    }

    if (strncmp(input, "/workspace/", 11) == 0) {
      snprintf(buf, buf_size, ".%s", input + 10);
      if (access(buf, F_OK) == 0) {
        return buf;
      }

      snprintf(buf, buf_size, "..%s", input + 10);
      if (access(buf, F_OK) == 0) {
        return buf;
      }
    }

    return input;
}

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
    case ET_NONE:
      type_str = "Unknown (ET_NONE)";
      break;
    case ET_REL:
      type_str = "Relocatable (ET_REL)";
      break;
    case ET_EXEC:
      type_str = "Executable (ET_EXEC)";
      break;
    case ET_DYN:
      type_str = "Shared Object/PIE (ET_DYN)";
      break;
    case ET_CORE:
      type_str = "Core Dump (ET_CORE)";
      break;
    default:
      if (e_type >= ET_LOOS && e_type <= ET_HIOS)
        type_str = "OS-Specific";
      else if (e_type >= ET_LOPROC)
        type_str = "Processor-Specific";
      else
        type_str = "Invalid";
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int __cmd_myfile(const char* filename) {
    char filepath[256];
  char fallback[512];
    int fd;
    Elf64_Ehdr ehdr;

  const char *resolved = resolve_path(filename, fallback, sizeof(fallback));
  strncpy(filepath, resolved, sizeof(filepath) - 1);
  filepath[sizeof(filepath) - 1] = '\0';
    fflush(stdout);
    printf("filepath: %s\n", filepath);

    // TODO: 在这里添加你的代码
    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
      perror("open");
      return 1;
    }

    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
      perror("read");
      close(fd);
      return 1;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "Not an ELF file: %s\n", filepath);
      close(fd);
      return 1;
    }

    print_elf_type(ehdr.e_type);
    close(fd);
    return 0;
}