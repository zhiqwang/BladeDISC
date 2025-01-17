#include "tao_bridge/kernels/process.h"

namespace tensorflow {
namespace tao {

timespec Process::StartTimeTS() {
  std::stringstream path;
  path << "/proc/" << pid_;
  struct stat statbuf;
  int ret = stat(path.str().c_str(), &statbuf);
  // use the dir modification time as the process start time
  if (ret == 0) return statbuf.st_mtim;

  timespec t;
  t.tv_nsec = 0;
  t.tv_sec = 0;
  return t;
}

time_t Process::StartTime() {
  timespec t = StartTimeTS();
  return t.tv_sec;
}

uint64_t Process::StartTimeUS() {
  timespec t = StartTimeTS();
  return (uint64_t)(t.tv_sec * 1e6 + t.tv_nsec / 1e3);
}

std::vector<std::string> Process::CMD() {
  std::vector<std::string> cmd;
  std::stringstream proc;
  proc << "/proc/" << pid_ << "/cmdline";

  char buf[1024];
  memset(buf, 0, sizeof(buf));
  std::ifstream cmdline(proc.str());
  if (!cmdline.is_open()) {
    return cmd;
  }

  while (cmdline.getline(buf, sizeof(buf) - 1, 0)) {
    cmd.push_back(std::string(buf));
  }
  return cmd;
}

std::string Process::CWD() {
  char buf[1024];
  memset(buf, 0, sizeof(buf));

  std::stringstream proc;
  proc << "/proc/" << pid_ << "/cwd";
  if (readlink(proc.str().c_str(), buf, sizeof(buf) - 1) < 0) {
    // readlink failed
    return "read cwd failed: " + std::string(strerror(errno));
  }
  return buf;
}

}  // namespace tao
}  // namespace tensorflow
