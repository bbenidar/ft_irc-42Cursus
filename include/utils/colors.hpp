/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:10:41 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/26 15:02:31 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
#define COLORS_HPP

# define BOLD        "\e[1m"
# define UNDERLINED  "\e[4m"

# define RESET_ALL         "\e[0m"
# define RESET_BOLD        "\e[21m"
# define RESET_UNDERLINED  "\e[24m"

# define FG_DEFAULT  "\e[39m"
# define FG_BLACK    "\e[30m"
# define FG_RED      "\e[31m"
# define FG_GREEN    "\e[32m"
# define FG_YELLOW   "\e[33m"
# define FG_BLUE     "\e[34m"
# define FG_MAGENTA  "\e[35m"
# define FG_CYAN     "\e[36m"
# define FG_LGRAY    "\e[37m"
# define FG_DGRAY    "\e[90m"
# define FG_LRED     "\e[91m"
# define FG_LGREEN   "\e[92m"
# define FG_LYELLOW  "\e[93m"
# define FG_LBLUE    "\e[94m"
# define FG_LMAGENTA "\e[95m"
# define FG_LCYAN    "\e[96m"
# define FG_WHITE    "\e[97m"

# define BG_DEFAULT  "\e[49m"
# define BG_BLACK    "\e[40m"
# define BG_RED      "\e[41m"
# define BG_GREEN    "\e[42m"
# define BG_YELLOW   "\e[43m"
# define BG_BLUE     "\e[44m"
# define BG_MAGENTA  "\e[45m"
# define BG_CYAN     "\e[46m"
# define BG_LGRAY    "\e[47m"
# define BG_DGRAY    "\e[100m"
# define BG_LRED     "\e[101m"
# define BG_LGREEN   "\e[102m"
# define BG_LYELLOW  "\e[103m"
# define BG_LBLUE    "\e[104m"
# define BG_LMAGENTA "\e[105m"
# define BG_LCYAN    "\e[106m"
# define BG_WHITE    "\e[107m"

# define INVALID_ARGUMENTS		BOLD FG_RED "Error: " FG_WHITE "invalid arguments."
# define VALID_INPUT			"Try ./ft_irc + \"Port number\" + \"Password\""

# define INVALID_PORT_NUMBER	BOLD FG_RED "Error: " FG_WHITE "invalid port number."
# define VALID_PORT_NUMBER		"The port number must be between 0 and 65535"

# define SOCKET_CREATION		BOLD FG_RED "Error: creating socket"

# define SOCKET_BINDING			BOLD FG_RED "Error: binding socket"

# define LISTENING_ERROR		BOLD FG_RED "Error: listening for connections"

# define ACCEPT_CONNECTION		BOLD FG_RED "Error: accepting connection"

# define POLL_FAILURE		BOLD FG_RED "Error: poll() failed"

#endif