#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <termios.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <term.h>

struct s_vars;
typedef int	(*t_func)(struct s_vars *vars, char **args);

typedef enum e_function
{
	MS_ECHO = 0,
	MS_CD = 1,
	MS_PWD = 2,
	MS_EXPORT = 3,
	MS_UNSET = 4,
	MS_ENV = 5,
	MS_EXIT = 6,
}	t_function;

typedef struct s_vars
{
	char	**envp;
	char	**functions;
	t_func	func[7];
}	t_vars;

void	ms_init_signal(void);

#endif