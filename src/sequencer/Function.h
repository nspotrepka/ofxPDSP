
// Function.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2020

#ifndef PDSP_LAMBDA_H_INCLUDED
#define PDSP_LAMBDA_H_INCLUDED

#include <functional>
#include <atomic>
#include <iostream>
#include "../DSP/pdspCore.h"
#include "../messages/header.h"
#include "../DSP/control/SequencerGateOutput.h"
#include "../DSP/control/SequencerValueOutput.h"

namespace pdsp{

    class Function {
        friend class SequencerProcessor;

        struct Out {
            std::string name;
            pdsp::MessageBuffer* messageBuffer;
            pdsp::SequencerGateOutput* gate_out;
            pdsp::SequencerValueOutput* value_out;
        };
        
    public:
        Function();
        ~Function();
    
        /*!
        @brief get a trigger output for the given name.
        @param[in] name output name tag
        */       
        pdsp::SequencerGateOutput& out_trig( std::string name );

        /*!
        @brief get a value output for the given name. 
        @param[in] name output name tag
        */    
        pdsp::SequencerValueOutput& out_value( std::string name  );

        /*!
        @brief this lambda function is executed each step, at the timing assigned to the class. Assign your own functions to it.
        The step number is passed to the function as int value. Remember that this function will be executed into the audio-thread so the access to some variable used also into the main thread could cause race conditions.
        */
        std::function<void(int)> code;

        /*!
        @brief timing for the function frames, defaults to 16 ( 1/16th of a bar ). Thread-safe to set.
        */
        std::atomic<int> timing;

        /*!
        @brief enable / disable exectution of lambda, true by default, Thread-safe to set.
        */
        std::atomic<bool> active;
        
        /*!
        @brief use this method inside the assigned code to send a value to a given output
        */ 
        void send( std::string name, float value ) noexcept;

        /*!
        @brief internal random, returns a value from 0.0f to 1.0f ( included ).
        */ 
        float urand();

        /*!
        @brief internal random, returns a value from -1.0f to 1.0f ( included ).
        */ 
        float brand();
        
        /*!
        @brief internal random, returns value from 0 to max-1
        @param[in] max max exclusive
        */ 
        int dice( int max );

        /*!
        @brief randomly returns true of false, with the argument setting the chance of being "true".
        @param[in] probablility chance of output being true
        */         
        bool chance( float probablility );
 
    private:
        std::atomic<int> sample;
    
        // random from https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
        // the max number is not RAND_MAX but a short int max
        int seed;
        inline int fastrand() { 
            seed = (214013*seed+2531011); 
            return (seed>>16)&0x7FFF; 
        } 
    
        void process( double playhead, double barsPerSample, int bufferSize );
        
        void clear( int bufferSize );

        std::vector<Out> outputs;            
        static pdsp::SequencerGateOutput invalidGate;
        static pdsp::SequencerValueOutput invalidValue;
        
        static std::vector<Function*> instances;
        
    };
    
}


#endif // PDSP_LAMBDA_H_INCLUDED
