/** original source at:
        https://rimstar.org/science_electronics_projects/backpropagation_neural_network_software_3_layer.htm
    @date   20190122_jintaeks
            converted to C++
*/
/*
* testcounting.c - This program tests the neural network library by
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
* is below. The decimal equivalent is given in parenthesis.
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
//#include <unistd.h>
#include <string.h>
#include "backprop.h"

int verbose = 0;

void print_all_network_info( int epoch );
int get_number( float *vals );
int parse_args( int argc, char **argv );

#define NUMINPUTS      3 /* number of input units */
#define NUMHIDDEN      3 /* number of hidden units */
#define NUMOUTPUTS     3 /* number of output units */
#define NUMINTRAINSET  8 /* number of values/epochs in the training set */
#define NUMOFEVALS     8 /* number of values in the test set */

float g_aInputVals[ NUMINTRAINSET ][ NUMINPUTS ] =
{
    { 0.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 1.0, 1.0 },
    { 1.0, 0.0, 0.0 },
    { 1.0, 0.0, 1.0 },
    { 1.0, 1.0, 0.0 },
    { 1.0, 1.0, 1.0 }
};

float g_aTargetVals[ NUMINTRAINSET ][ NUMOUTPUTS ] =
{
    { 0.0, 0.0, 1.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 1.0, 1.0 },
    { 1.0, 0.0, 0.0 },
    { 1.0, 0.0, 1.0 },
    { 1.0, 1.0, 0.0 },
    { 1.0, 1.0, 1.0 },
    { 0.0, 0.0, 0.0 }
};

float g_aTestInputVals[ NUMOFEVALS ][ NUMINPUTS ] =
{
    { 0.0, 1.0, 0.0 },
    { 0.0, 1.0, 1.0 },
    { 0.0, 0.0, 1.0 },
    { 1.0, 1.0, 1.0 },
    { 1.0, 0.0, 1.0 },
    { 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 0.0 },
    { 1.0, 1.0, 0.0 }
};

float g_fLastLearningError = 0.f;
float g_fLastRMSError = 0.f;
float g_aResultInputVals[ NUMINPUTS ]; // input value
float g_aResultIHWeights[ NUMINPUTS*NUMHIDDEN ]; // input -> hidden weights
float g_aResultHiddenVals[ NUMHIDDEN ]; // hidden value
float g_aResultHOWeights[ NUMHIDDEN*NUMOUTPUTS ]; // hidden -> output weights
float g_aResultOutputVals[ NUMOUTPUTS ]; // output value
float g_aResultBHWeights[ 1 * NUMHIDDEN ]; // bias -> hidden weights
float g_aResultBiasVals[ 1 ]; // bias value
float g_aResultBOWeights[ 1 * NUMOUTPUTS ]; // bias -> output weights

int main( int argc, char **argv ) {
    int             epoch = 0;
    KNeuralNetwork*  net;
    KNeuralNetworkConfig    config;
    int             iError = 0;

    if( argc == 2 && argv[ 1 ][ 0 ] == 'v' ) {
        verbose = 1;
    }

    config.Type = BACKPROP_TYPE_NORMAL;
    config.m_NumInputs = NUMINPUTS;
    config.m_NumHidden = NUMHIDDEN;
    config.m_NumOutputs = NUMOUTPUTS;
    config.m_StepSize = 0.25f;
    config.m_Momentum = 0.90f;
    config.m_Cost = 0.0;

    net = new KNeuralNetwork();
    iError = net->CreateNetwork(&config);
    if( iError == -1 ) {
        perror( "CreateNetwork() failed" );
        exit( EXIT_FAILURE );
    }

    iError = net->SetTrainingSet(NUMINTRAINSET, (float *)g_aInputVals, (float *)g_aTargetVals);
    if( iError  == -1 ) {
        perror( "SetTrainingSet() failed" );
        exit( EXIT_FAILURE );
    }

    g_fLastRMSError = 99;
    for( epoch = 1; g_fLastRMSError > 0.0005 && epoch <= 1000000; epoch++ ) {
        iError = net->Learn(1);
        if( iError == -1 ) {
            perror( "Learn() failed" );
            exit( EXIT_FAILURE );
        }
        if( verbose == 0 ) {
            iError = net->Query(NULL, &g_fLastRMSError, NULL
                , NULL, NULL, NULL, NULL, NULL, NULL, NULL );
            if( iError == -1 ) {
                perror( "Query() failed" );
            }
        } else  if( verbose == 1 ) {
            /* The following prints everything in the neural network for
            each step during the learning. It's just to illustrate
            that you can examine everything that's going on in order to
            debug or fine tune the network.
            Warning: Doing all this printing will slow things down
            significantly.  */
            iError = net->Query( &g_fLastLearningError, &g_fLastRMSError, g_aResultInputVals
                , g_aResultIHWeights, g_aResultHiddenVals, g_aResultHOWeights
                , g_aResultOutputVals
                , g_aResultBHWeights, g_aResultBiasVals, g_aResultBOWeights );
            if( iError == -1 ) {
                perror( "Query() failed" );
            }
            print_all_network_info(epoch);
        }
    }

    {
        /* The following prints everything in the neural network after
        the last learned training set. */
        iError = net->Query( &g_fLastLearningError, &g_fLastRMSError, g_aResultInputVals
            , g_aResultIHWeights, g_aResultHiddenVals, g_aResultHOWeights
            , g_aResultOutputVals
            , g_aResultBHWeights, g_aResultBiasVals, g_aResultBOWeights );
        if( iError == -1 ) {
            perror( "Query() failed" );
        }
        print_all_network_info(epoch);
    }

    printf( "Stopped learning at epoch: %d, RMS Error: %f.\n"
        , epoch - 1, g_fLastRMSError );

    printf( "\n=======================================================\n" );
    printf( "First, some quick tests, giving it binary numbers to see if it "
        "spits\nout the next number.\n" );
    for( int i = 0; i < NUMOFEVALS; i++ ) {
        printf( "Evaluating inputs:" );
        for( int j = 0; j < NUMINPUTS; j++ )
            printf( " %f", g_aTestInputVals[ i ][ j ] );
        printf( ", %d\n", get_number( g_aTestInputVals[ i ] ) );

        iError = net->SetInput(0, 0.0, g_aTestInputVals[i]);
        if( iError == -1 ) {
            perror( "SetInput() failed" );
            exit( EXIT_FAILURE );
        }
        iError = net->Evaluate(g_aResultOutputVals, NUMOUTPUTS*sizeof(float));
        if( iError == -1 ) {
            perror( "Evaluate() failed" );
            exit( EXIT_FAILURE );
        }
        printf( "Output values:" );
        for( int j = 0; j < NUMOUTPUTS; j++ )
            printf( " %f", g_aResultOutputVals[ j ] );
        printf( ", %d\n", get_number( g_aResultOutputVals ) );
        printf( "\n" );
    }

    net->DestroyNetwork();
    delete net;

    return 0;
}

void print_all_network_info( int epoch )
{
    int j;

    printf( "m_Epoch: %d  RMS Error: %f  Learning Error: %f\nInput values: "
        , epoch, g_fLastRMSError, g_fLastLearningError );
    for( j = 0; j < NUMINPUTS; j++ )
        printf( "%f ", g_aResultInputVals[ j ] );
    printf( "\nInput to Hidden Weights: " );
    for( j = 0; j < NUMINPUTS*NUMHIDDEN; j++ )
        printf( "%f ", g_aResultIHWeights[ j ] );
    printf( "\nHidden unit values: " );
    for( j = 0; j < NUMHIDDEN; j++ )
        printf( "%f ", g_aResultHiddenVals[ j ] );
    printf( "\nHidden to Output Weights: " );
    for( j = 0; j < NUMHIDDEN*NUMOUTPUTS; j++ )
        printf( "%f ", g_aResultHOWeights[ j ] );
    printf( "\nLast Outputs values for the last m_Epoch: " );
    for( j = 0; j < NUMOUTPUTS; j++ )
        printf( "%f ", g_aResultOutputVals[ j ] );
    printf( "\nBias to Hidden Weights: " );
    for( j = 0; j < 1 * NUMHIDDEN; j++ )
        printf( "%f ", g_aResultBHWeights[ j ] );
    printf( "\nBias unit values: " );
    for( j = 0; j < 1; j++ )
        printf( "%f ", g_aResultBiasVals[ j ] );
    printf( "\nBias to Output Weights: " );
    for( j = 0; j < 1 * NUMOUTPUTS; j++ )
        printf( "%f ", g_aResultBOWeights[ j ] );
    printf( "\n\n" );
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
int get_number( float *vals )
{
    int num = 0, i;

    for( i = 0; i < NUMOUTPUTS; i++ )
        if( vals[ i ] > 0.05 )
            num += 1 << ( ( NUMOUTPUTS - i ) - 1 );
    return num;
}
