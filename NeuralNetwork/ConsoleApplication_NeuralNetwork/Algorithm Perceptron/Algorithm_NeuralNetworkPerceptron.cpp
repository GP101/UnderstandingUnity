#include <stdio.h>
#include <iostream>
#include <memory>
#include <time.h>
#include <vector>

/*  @desc   random()
    for (int i = 0; i < 10; ++i) {
        float r = random(-1.f, +1.f);
        printf("%g\r\n", r);
    }//for */
float random( float fmin, float fmax )
{
    float f = (float)rand() / (float)RAND_MAX;
    return f * (fmax - fmin) + fmin;
}

class Perceptron
{
private:
    std::vector<float>  spWeights;
    int mWeightsSize = 0;
    float c = 0.01; // learning rate

public:
    Perceptron(int n)
    {
        mWeightsSize = n;
        spWeights.resize(mWeightsSize);
        for (int i = 0; i < mWeightsSize; i++) {
            spWeights[ i ] = random( -1, 1 );
        }
    }

    int feedforward(float inputs[])
    {
        float sum = 0;
        for (int i = 0; i < mWeightsSize; i++) {
            sum += inputs[ i ] * spWeights[ i ];
        }
        return activate(sum);
    }

    int activate(float sum)
    {
        if (sum > 0) return 1;
        else return -1;
    }

    void train(float inputs[], int desired)
    {
        int guess = feedforward(inputs);
        float error = desired - guess;
        for (int i = 0; i < mWeightsSize; i++) {
            spWeights[ i ] += c * error * inputs[ i ];
        }
    }
};

class Trainer
{
public:
    //A "Trainer" object stores the inputs and the correct answer.
    float mInputs[ 3 ];
    int mAnswer;

    void SetData( float x, float y, int a )
    {
        mInputs[ 0 ] = x;
        mInputs[ 1 ] = y;
        //Note that the Trainer has the bias input built into its array.
        mInputs[ 2 ] = 1;
        mAnswer = a;
    }

    ~Trainer()
    {
    }
};

std::shared_ptr<Perceptron> spPerceptron;
const int gTrainerSize = 2000; //2,000 training points
Trainer gTraining[ gTrainerSize ];
int gWidth = 640;
int gHeight = 640;

//The formula for a line
float f( float x )
{
    return 2 * x + 1.0f;
}

void Setup()
{
    srand( time( 0 ) );
    //size( 640, 360 );
    spPerceptron.reset( new Perceptron( 3 ) );

    //Make 2,000 training points.
    for( int i = 0; i < gTrainerSize; i++ ) {
        float x = random( -gWidth / 2, gWidth / 2 );
        float y = random( -gHeight / 2, gHeight / 2 );
        //Is the correct answer 1 or - 1 ?
        int answer = 1;
        if( y < f( x ) ) answer = -1;
        gTraining[ i ].SetData( x, y, answer );
    }
}

void Training()
{
    for( int i = 0; i < gTrainerSize; i++ ) {
        spPerceptron->train( gTraining[ i ].mInputs, gTraining[ i ].mAnswer );
    }
}

void main()
{
    Setup();
    Training();

    for( int i = 0; i < 20; ++i ) {
        const float x = random( -gWidth / 2, gWidth / 2 );
        const float y = random( -gHeight / 2, gHeight / 2 );
        float inputs[] = { x, y, 1 };
        const int nnResult = spPerceptron->feedforward( inputs );
        printf( "(%g,%g)-> actual %i, perceptron %i\r\n"
            , x, y, y >= f( x ) ? 1 : -1, nnResult );
        //getch();
    }//for

    //float inputs[] = { 2, 6, 1 };
    //printf( "%i\r\n", spPerceptron->feedforward( inputs ) );
    //float inputs2[] = { -6, -10, 1 };
    //printf( "%i\r\n", spPerceptron->feedforward( inputs2 ) );
    //float inputs3[] = { -6, -12, 1 };
    //printf( "%i\r\n", spPerceptron->feedforward( inputs3 ) );
}
