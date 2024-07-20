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

int new_child(void)
{
    pid_t   pid;

    pid = fork();
    if (pid < 0)
        return (1);
    else if (pid == 0)
    {
        sleep(20);
        exit(1);
    }
    return (pid);
}

int main(void)
{
    pid_t pids[5];
    int status;

    pids[0] = new_child();
    printf("all spawned\n");
    sleep(4);
    while (1)
    {
        int ret = waitpid(0, &status, WNOHANG);
        printf("ret is %d\n", ret);
        sleep(1);
    }
    return (0);
}