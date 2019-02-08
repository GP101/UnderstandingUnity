/*
 * testcounting.c - This program tests the neural network libary by
 * teaching it to count from 0 to 7 in binary.
 *
 * April 7, 2016 - Reduced the number of hidden units from 20 to 3.
 *
 * It structures the neural network with 3 inputs, 3 hidden layers
 * and 3 outputs.
 *
 * The 3 inputs represent binary digits and are each either 0 or 1.
 * Since there are 3 of them, that means they can represent:
 * 000, 001, 010, 011, 100, 101, 110, 111
 * which are the binary equivalents of the digital numbers:
 *   0,   1,   2,   3,   4,   5,   6,   7
 *
 * This program trains the network such that for the given input
 * 000, it should set the three output units to 001. For the given
 * input 001, it should set the three output units to 010 (which is
 * decimal is 2), and so on... The table of trained inputs to outputs
 * is below. The decimal equivalent is given in paranthesis.
 *  input   output
 * -------  -------
 * 000 (0)  001 (1)
 * 001 (1)  010 (2)
 * 010 (2)  011 (3)
 * 011 (3)  100 (4)
 * 100 (4)  101 (5)
 * 101 (5)  110 (6)
 * 110 (6)  111 (7)
 * 111 (7)  000 (0)
 *
 * This means that given any number from 0 to 6, in binary, it will
 * output the next highest number. The exception is 7, wherein it will
 * output 0. The cool thing is that if you give it 0 and from then on
 * just take its output and give it right back as the input for the next
 * time around, all the while displaying the outputs, it will count
 * from 0 to 7 and repeat continuously i.e. it will count.
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "backprop.h"

static int verbose = 0;

static void print_all_network_info(int epoch);
static int get_number(float *vals);
static int parse_args(int argc, char **argv);

#define NUMINPUTS      3 /* number of input units */
#define NUMHIDDEN      3 /* number of hidden units */
#define NUMOUTPUTS     3 /* number of output units */
#define NUMINTRAINSET  8 /* number of values/epochs in the training set */
#define NUMOFEVALS     8 /* number of values in the test set */

static float InputVals[NUMINTRAINSET][NUMINPUTS] = {
   { 0.0, 0.0, 0.0 }, 
   { 0.0, 0.0, 1.0 }, 
   { 0.0, 1.0, 0.0 }, 
   { 0.0, 1.0, 1.0 },
   { 1.0, 0.0, 0.0 },
   { 1.0, 0.0, 1.0 },
   { 1.0, 1.0, 0.0 },
   { 1.0, 1.0, 1.0 }
};
static float TargetVals[NUMINTRAINSET][NUMOUTPUTS] = {
   { 0.0, 0.0, 1.0 }, 
   { 0.0, 1.0, 0.0 }, 
   { 0.0, 1.0, 1.0 },
   { 1.0, 0.0, 0.0 },
   { 1.0, 0.0, 1.0 },
   { 1.0, 1.0, 0.0 },
   { 1.0, 1.0, 1.0 },
   { 0.0, 0.0, 0.0 } 
};
static float TestInputVals[NUMOFEVALS][NUMINPUTS] = {
   { 0.0, 1.0, 0.0 }, 
   { 0.0, 1.0, 1.0 },
   { 0.0, 0.0, 1.0 }, 
   { 1.0, 1.0, 1.0 },
   { 1.0, 0.0, 1.0 },
   { 1.0, 0.0, 0.0 },
   { 0.0, 0.0, 0.0 }, 
   { 1.0, 1.0, 0.0 }
};
float TestCountFromZero[NUMINPUTS] = {
   0.0, 0.0, 0.0,
};

float LastLearningError;
float LastRMSError;
float ResultInputVals[NUMINPUTS]; 
float ResultIHWeights[NUMINPUTS*NUMHIDDEN];
float ResultHiddenVals[NUMHIDDEN];
float ResultHOWeights[NUMHIDDEN*NUMOUTPUTS];
float ResultOutputVals[NUMOUTPUTS];
float ResultBHWeights[1*NUMHIDDEN];
float ResultBiasVals[1];
float ResultBOWeights[1*NUMOUTPUTS];

int
main(int argc, char **argv)
{
   int i, j, epoch, fd;
   bkp_network_t *net;
   bkp_config_t config;

   if (parse_args(argc, argv) == -1)
      exit(EXIT_FAILURE);

   config.Type = BACKPROP_TYPE_NORMAL;
   config.NumInputs = NUMINPUTS;
   config.NumHidden = NUMHIDDEN;
   config.NumOutputs = NUMOUTPUTS;
   config.StepSize = 0.25;
   config.Momentum = 0.90;
   config.Cost = 0.0;
   if (bkp_create_network(&net, &config) == -1) {
      perror("bkp_create_network() failed");
      exit(EXIT_FAILURE);
   }	

   if (bkp_set_training_set(net, NUMINTRAINSET, (float *) InputVals, (float *) TargetVals) == -1) {
      perror("bkp_set_training_set() failed");
      exit(EXIT_FAILURE);
   }

   LastRMSError = 99;
   for (epoch = 1;  LastRMSError > 0.0005  &&  epoch <= 1000000;  epoch++) {
      if (bkp_learn(net, 1) == -1) {
         perror("bkp_learn() failed");
         exit(EXIT_FAILURE);
      }
      if (verbose <= 1) {
         if (bkp_query(net, NULL, &LastRMSError, NULL,
               NULL, NULL, NULL, NULL, NULL, NULL, NULL) == -1) {
            perror("bkp_query() failed");
         }
      } else  if (verbose == 2) {
         /* The following prints everything in the neural network for
            each step during the learning. It's just to illustrate
            that you can examine everything that's going on in order to
            debug or fine tune the network.
            Warning: Doing all this printing will slow things down
            significantly.  */
         if (bkp_query(net, &LastLearningError, &LastRMSError, ResultInputVals,
               ResultIHWeights, ResultHiddenVals, ResultHOWeights, 
               ResultOutputVals,
               ResultBHWeights, ResultBiasVals, ResultBOWeights) == -1) {
            perror("bkp_query() failed");
         }
         print_all_network_info(epoch);
      }
   }
   if (verbose == 1) {
      /* The following prints everything in the neural network after
         the last learned training set. */
      if (bkp_query(net, &LastLearningError, &LastRMSError, ResultInputVals,
            ResultIHWeights, ResultHiddenVals, ResultHOWeights, 
            ResultOutputVals,
            ResultBHWeights, ResultBiasVals, ResultBOWeights) == -1) {
         perror("bkp_query() failed");
      }
      print_all_network_info(epoch);
   }

   printf("Stopped learning at epoch: %d, RMS Error: %f.\n",
      epoch-1, LastRMSError);

   printf("\n");
   printf("=======================================================\n");
   printf("First, some quick tests, giving it binary numbers to see if it " 
      "spits\nout the next number.\n");
   for (i = 0;  i < NUMOFEVALS;  i++) {
      printf("Evaluating inputs:");
      for (j = 0;  j < NUMINPUTS;  j++)
         printf(" %f", TestInputVals[i][j]);
      printf(", %d\n", get_number(TestInputVals[i]));

      if (bkp_set_input(net, 0, 0.0, TestInputVals[i]) == -1) {
         perror("bkp_set_input() failed");
         exit(EXIT_FAILURE);
      }
      if (bkp_evaluate(net, ResultOutputVals, NUMOUTPUTS*sizeof(float)) == -1) {
         perror("bkp_evaluate() failed");
         exit(EXIT_FAILURE);
      }
      printf("Output values:");
      for (j = 0;  j < NUMOUTPUTS;  j++)
         printf(" %f", ResultOutputVals[j]);
      printf(", %d\n", get_number(ResultOutputVals));
      printf("\n");
   }	

   printf("=======================================================\n");
   printf("Next, getting it to count by using the outputs for the next inputs.\n");
   /* NOTE: Starting from 0.0, 0.0, 1.0 is better for some reason.
      0.0, 0.0, 0.0 -> 0.0, 0.0, 1.0 is not learned very well. */
   printf("Making it count from binary 000 to 111.\n");
   printf("Giving it the following for the first input:");
   for (j = 0;  j < NUMINPUTS;  j++)
      printf(" %f", TestCountFromZero[j]);
   printf(", %d\n", get_number(TestCountFromZero));
   if (bkp_set_input(net, 0, 0.0, TestCountFromZero) == -1) {
      perror("bkp_set_input() failed");
      exit(EXIT_FAILURE);
   }
   for (i = 0;  i < 3*NUMOFEVALS;  i++) {
      if (bkp_evaluate(net, ResultOutputVals, NUMOUTPUTS*sizeof(float)) == -1) {
         perror("bkp_evaluate() failed");
         exit(EXIT_FAILURE);
      }
      printf("Ouput values, and also the next input values:");
      for (j = 0;  j < NUMOUTPUTS;  j++)
         printf(" %f", ResultOutputVals[j]);
      printf(", %d\n", get_number(ResultOutputVals));
      if (bkp_set_input(net, 0, 0.0, ResultOutputVals) == -1) {
         perror("bkp_set_input() failed");
         exit(EXIT_FAILURE);
      }
   }

   bkp_destroy_network(net);

   return 0;
}

static void print_all_network_info(int epoch)
{
   int j;

   printf("Epoch: %d  RMS Error: %f  Learning Error: %f\nInput values: ", 
         epoch, LastRMSError, LastLearningError);
   for (j = 0;  j < NUMINPUTS;  j++)
      printf("%f ", ResultInputVals[j]);
   printf("\nInput to Hidden Weights: ");
   for (j = 0;  j < NUMINPUTS*NUMHIDDEN;  j++)
      printf("%f ", ResultIHWeights[j]);
   printf("\nHidden unit values: ");
   for (j = 0;  j < NUMHIDDEN;  j++)
      printf("%f ", ResultHiddenVals[j]);
   printf("\nHidden to Output Weights: ");
   for (j = 0;  j < NUMHIDDEN*NUMOUTPUTS;  j++)
      printf("%f ", ResultHOWeights[j]);
   printf("\nLast Outputs values for the last Epoch: ");
   for (j = 0;  j < NUMOUTPUTS;  j++)
      printf("%f ", ResultOutputVals[j]);
   printf("\nBias to Hidden Weights: ");
   for (j = 0;  j < 1*NUMHIDDEN;  j++)
      printf("%f ", ResultBHWeights[j]);
   printf("\nBias unit values: ");
   for (j = 0;  j < 1;  j++)
      printf("%f ", ResultBiasVals[j]);
   printf("\nBias to Output Weights: ");
   for (j = 0;  j < 1*NUMOUTPUTS;  j++)
      printf("%f ", ResultBOWeights[j]);
   printf("\n\n");
}

/*
 * get_number - Given an array of three floating point values, each
 * in the range 0.0 to 1.0, this will convert any value <= 0.05 to
 * a 0 and any > 0.05 to a 1 and treat the resulting three 0s and 1s
 * as three binary digits. It will then return the digital equivalent.
 *
 * Example:
 * Given an array containing 0.00000, 0.94734, 0.02543, these will be 
 * converted to 0, 1, 0 or 010 which is a binary number that is 2 in 
 * digital. This will therefore return a 2.
 */
static int get_number(float *vals)
{
   int num = 0, i;

   for (i = 0; i < NUMOUTPUTS; i++)
      if (vals[i] > 0.05)
         num += 1 << ((NUMOUTPUTS-i)-1);
   return num;
}

static int parse_args(int argc, char **argv)
{
   int c;

   while ((c = getopt(argc, argv, "v")) != -1) {
      switch (c) {
      case 'v': verbose++; break;
      case '?': exit(EXIT_FAILURE);
      }
   }
   if (argv[optind] != NULL) {
      fprintf(stderr, "Invalid argument.\n");
      return -1;
   }
   return 0;
}
