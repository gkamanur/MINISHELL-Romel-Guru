/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:36:16 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/06 11:43:14 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// debug_logger.c


#include "../includes/minishell.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void log_debug(const char *fmt, ...)
{
	static FILE *log_file = NULL;
	va_list args;
	time_t now;
	char time_buf[64];

	// Open the file only once
	if (!log_file)
		log_file = fopen("debug.log", "w"); // "w" -> overwrite each run, use "a" to append

	if (!log_file)
		return; // prevent crash if file can't be opened

	// Timestamp
	time(&now);
	strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", localtime(&now));
	fprintf(log_file, "%s ", time_buf);

	// Variadic log message
	va_start(args, fmt);
	vfprintf(log_file, fmt, args);
	va_end(args);

	fprintf(log_file, "\n");
	fflush(log_file); // flush immediately for real-time debugging
}
