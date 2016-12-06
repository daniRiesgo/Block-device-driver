
/*
 *  boot.c
 *
 *  DrvSim. Version 2.0
 *
 *  INFODSO@ARCOS.INF.UC3M.ES
 *
 */

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>

int main ( int argc, char *argv[] )
{
    pid_t driver_id, diskhw_id, client_id ;
    int   i, j, n_clients, test_number;
    int   status;
    int  *children;
    int   wait_return;
    char  argv_name[1024] ;
    char  argv_1[1024] ;
    char *argv_clients[4] ;


    /* 1) check/get parameters... */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number of_clients>\n", argv[0]) ;
        exit(-1) ;
    }

    test_number = atoi(argv[1]);

    switch (test_number) {
        case 1: // ids 6,7,7,7
            n_clients = 4;
            break;
        case 2: // ids default
            n_clients = 3;
            break;
        case 3: // id default
            n_clients = 1;
            break;
        case 4: // ids 1,2,3,4
            n_clients = 4;
            break;
        case 5: // id 5
            n_clients = 1;
            break;
        case 6: // ids 8,9
            n_clients = 2;
            break;
        default:
            n_clients = test_number;
            break;
    }

    children = malloc(n_clients * sizeof(int));


    /* 2) boot disk (simulation) */
    printf("BOOT: initializing the disk hardware...\n");
    diskhw_id = fork() ;
    switch (diskhw_id)
    {
        case -1: // error
                 perror("ERROR in fork") ;
                 exit(-1) ;
                 break ;

        case  0: // son
		 sprintf(argv_name, "disk_hw") ;
                 argv_clients[0] = argv_name ;
                 argv_clients[1] = NULL ;

                 execvp("./disk_hw", argv_clients);
                 perror("execvp: ") ;
                 exit(-1) ;
                 break ;

        default: // father
                 break ;
    }

    /* 3) boot pkernel (simulation) */
    printf("BOOT: initializing the pkernel...\n");

    sleep(1) ;
    driver_id = fork() ;
    switch (driver_id)
    {
        case -1: // error
                 perror("ERROR in fork") ;
                 exit(-1) ;
                 break ;

        case  0: // son
		 sprintf(argv_name, "pkernel") ;

                 argv_clients[0] = argv_name ;
                 argv_clients[1] = NULL ;

                 execvp("./pkernel", argv_clients);
                 perror("execvp: ") ;
                 exit(-1) ;
                 break ;
    }


    /* 4) start up clients (simulation) */
    printf("BOOT: creating the clients\n");

    // Create as clients as wanted
    switch (test_number) {
        case 1: {
            for(i = 0; i < n_clients; i++)
            {
                client_id = fork() ;
                children[i] = client_id;
                switch (client_id)
                {
                    case -1: // error
                          perror("ERROR in fork") ;
                          exit(-1) ;
                          break ;

                    case  0: // son
                        sprintf(argv_name, "client") ;
                        if(i == 0) {
                            sprintf(argv_1,    "%d", 6) ;
                        } else {
                            sprintf(argv_1,    "%d", 7) ;
                        }

                        argv_clients[0] = argv_name ;
                        argv_clients[1] = argv_1 ;
                        argv_clients[2] = NULL ;

                        execvp("./client", argv_clients);
                        perror("execvp: ") ;
                        exit(-1) ;
                        break ;

                    default: // father
                          break ;
                }
            }
            break;
        }

        case 4: {
            for(i = 1; i <= n_clients; i++)
            {
                client_id = fork() ;
                children[i] = client_id;
                switch (client_id)
                {
                    case -1: // error
                          perror("ERROR in fork") ;
                          exit(-1) ;
                          break ;

                    case  0: // son
                        sprintf(argv_name, "client") ;
                        sprintf(argv_1,    "%d", i) ;

                        argv_clients[0] = argv_name ;
                        argv_clients[1] = argv_1 ;
                        argv_clients[2] = NULL ;

                        execvp("./client", argv_clients);
                        perror("execvp: ") ;
                        exit(-1) ;
                        break ;

                    default: // father
                          break ;
                }
            }
            break;
        }

        case 5:
            client_id = fork() ;
            children[0] = client_id;
            switch (client_id) {
                case -1: // error
                      perror("ERROR in fork") ;
                      exit(-1) ;
                      break ;

                case  0: // son
                    sprintf(argv_name, "client") ;
                    sprintf(argv_1,    "%d", 5) ;

                    argv_clients[0] = argv_name ;
                    argv_clients[1] = argv_1 ;
                    argv_clients[2] = NULL ;

                    execvp("./client", argv_clients);
                    perror("execvp: ") ;
                    exit(-1) ;
                    break ;

                default: // father
                      break ;
            }

            break;

        case 6: {
            for(i = 0; i < n_clients; i++)
            {
                client_id = fork() ;
                children[i] = client_id;
                switch (client_id)
                {
                    case -1: // error
                          perror("ERROR in fork") ;
                          exit(-1) ;
                          break ;

                    case  0: // son
                        sprintf(argv_name, "client") ;
                        if(i == 0) {
                            sprintf(argv_1,    "%d", 8) ;
                        } else {
                            sprintf(argv_1,    "%d", 9) ;
                        }

                        argv_clients[0] = argv_name ;
                        argv_clients[1] = argv_1 ;
                        argv_clients[2] = NULL ;

                        execvp("./client", argv_clients);
                        perror("execvp: ") ;
                        exit(-1) ;
                        break ;

                    default: // father
                          break ;
                }
            }
            break;
        }


        default: // ids from 10 on
        {
            for(i = 0; i < n_clients; i++)
            {
            	client_id = fork() ;
            	children[i] = client_id;
            	switch (client_id)
            	{
            	    case -1: // error
            		      perror("ERROR in fork") ;
            		      exit(-1) ;
            		      break ;

            	    case  0: // son
            		      sprintf(argv_name, "client") ;
            		      sprintf(argv_1,    "%d", i+10) ;

            		      argv_clients[0] = argv_name ;
            		      argv_clients[1] = argv_1 ;
            		      argv_clients[2] = NULL ;

            		      execvp("./client", argv_clients);
            		      perror("execvp: ") ;
            		      exit(-1) ;
            		      break ;

            	    default: // father
            		      break ;
            	}
            }
            break;
        }
    }

    /* 5) Wait for all processes */
    for(j=0; j<n_clients; j++)
    {
    	wait_return = waitpid(children[j], &status, 0);
    	printf("BOOT: process %d returned %d, status %d\n", children[j], wait_return, status);
    }
    kill(diskhw_id, SIGINT);
    kill(driver_id, SIGINT);


    /* 6) end */
    return 1;
}
