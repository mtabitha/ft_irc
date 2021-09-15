#include "Server.hpp"

int	x_int(int err, int res, const char *str, const char *file, int line)
{
  if (res == err)
    {
      std::cerr << str << " error (" << file << ", " << line << "): " << strerror(errno) << std::endl;
      exit (EXIT_FAILURE);
    } 
  return (res);
}

void	*x_void(void *err, void *res, const char *str, const char *file, int line)
{
  if (res == err)
    {
      fprintf(stderr, "%s error (%s, %d): %s\n", 
	      str, file, line, strerror(errno));
      exit (EXIT_FAILURE);
    } 
  return (res);
}