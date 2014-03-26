/* file.h */

long open_dir (char * f_spec, char * fname);
int	next_dir (long dh, char * fname);
void	close_dir (long dh);
