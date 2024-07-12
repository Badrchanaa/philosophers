/* *********************************************************************************************** */
/*                                                                                                 */
/*                                                        :::   ::::::::   ::::::::  :::::::::::   */
/*  test.c                                               :+:+:  :+:    :+: :+:    :+: :+:     :+:  */
/*                                                      +:+         +:+        +:+        +:+      */
/*  By: bchanaa <test@test.com>                         +#+      +#++:      +#++:        +#+       */
/*                                                     +#+         +#+        +#+      +#+         */
/*  Created: 18/05/2024 10:10:29 PM by bchanaa        #+#  #+#    #+# #+#    #+#     #+#           */
/*  Updated: 18/05/2024 10:10:42 PM by bchanaa     ####### ########   ########      ###.ma         */
/*                                                                                                 */
/* *********************************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t   pid;

    pid = fork();
    if (pid < 0)
        return (1);
    else if (pid == 0)
    {
        sleep(5);
        exit(127);
    }
    else
    {
        int stillAlive = 1;
        int status = 0;
        while (stillAlive)
        {
            sleep(1);
            if (waitpid(pid, &status, WNOHANG) > 0) // TERMINATED;
              stillAlive = 0; 
            printf("status: %d still alive: %d\n", status, stillAlive);
        }
    }
    return (0);
}