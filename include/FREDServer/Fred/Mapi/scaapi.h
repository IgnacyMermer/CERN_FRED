#ifndef GBTSCAAPI_H
#define GBTSCAAPI_H

#include <tuple>
#include <boost/format.hpp>
#include <chrono>
#include "Parser/utility.h"
#include "Alfred/print.h"

using namespace std;

namespace gbtscaapi
{

class ScaApi
{
private:
	std::string sequence;
	std::vector<std::tuple<int,int>> sequenceConstruction;

	inline std::string constructPacket(int ch, int trid, int len, int cmd, int data = 0)
	{
		return (boost::format("0x%02x%02x%02x%02x,0x%08x\n") % ch % trid % len % cmd % data).str();
	}
	
	inline std::string getScaOperationValue(int scaOpType, int length, std::string alfResponsePart)
	{
		if( scaOpType == SCA_SVL_CONNECT){
			return "0";
		}
		else if (length == 1)
		{
			return Utility::splitString(alfResponsePart, ",")[1];
		}
		else if (scaOpType == I2C_M_7B_R)
		{
			std::string response;
			std::vector<std::string> splitAlfResponseParts = Utility::splitString(alfResponsePart, "\n");
			// Ignore the first response (was W_CMD), take the rest
			for (int i = 1; i++; i < length)
			{
				std::string s = Utility::splitString(splitAlfResponseParts[i],",")[1];
				response.append(s);
			}
			return response;
		}
		else if (scaOpType == I2C_M_7B_W)
		{
			std::string response;
			std::vector<std::string> splitAlfResponseParts = Utility::splitString(alfResponsePart, "\n");
			// We only get information in the last line
			response.append(Utility::splitString(splitAlfResponseParts[length-1],",")[1]);
			return response;
		}
	}

protected:


public:

	static const uint8_t CTRL_R_ID = 0xD1;
        static const uint8_t CTRL_W_CRB = 0x02;
        static const uint8_t CTRL_R_CRB = 0x03;
        static const uint8_t CTRL_W_CRC = 0x04;
        static const uint8_t CTRL_R_CRC = 0x05;
        static const uint8_t CTRL_W_CRD = 0x06;
        static const uint8_t CTRL_R_CRD = 0x07;
	static const uint8_t SCA_WAIT = 0xFF;
        static const uint8_t SCA_SC_RESET = 0xFF;
        static const uint8_t SCA_SVL_RESET = 0xFF;
        static const uint8_t SCA_SVL_CONNECT = 0xFF;

	// I2C Channels (not used)
	static const uint8_t I2C0 = 0x03;
	static const uint8_t I2C1 = 0x04;
	static const uint8_t I2C2 = 0x05;
	static const uint8_t I2C3 = 0x06;
	static const uint8_t I2C4 = 0x07;
	static const uint8_t I2C5 = 0x08;
	static const uint8_t I2C6 = 0x09;
	static const uint8_t I2C7 = 0x0A;
	static const uint8_t I2C8 = 0x0B;
	static const uint8_t I2C9 = 0x0C;
	static const uint8_t I2CA = 0x0D;
	static const uint8_t I2CB = 0x0E;
	static const uint8_t I2CC = 0x0F;
	static const uint8_t I2CD = 0x10;
	static const uint8_t I2CE = 0x11;
	static const uint8_t I2CF = 0x12;
	// I2C Operations (values are exact CMD values in GBT_SCA Manual)
	static const uint8_t I2C_W_CTRL = 0x30;
	static const uint8_t I2C_R_CTRL = 0x31;
	static const uint8_t I2C_R_STR = 0x11;
	static const uint8_t I2C_W_MSK = 0x20;
	static const uint8_t I2C_R_MSK = 0x21;
	static const uint8_t I2C_W_DATA0 = 0x40;
	static const uint8_t I2C_R_DATA0 = 0x41;
	static const uint8_t I2C_W_DATA1 = 0x50;
	static const uint8_t I2C_R_DATA1 = 0x51;
	static const uint8_t I2C_W_DATA2 = 0x60;
	static const uint8_t I2C_R_DATA2 = 0x61;
	static const uint8_t I2C_W_DATA3 = 0x70;
	static const uint8_t I2C_R_DATA3 = 0x71;
	static const uint8_t I2C_S_7B_W = 0x82;
	static const uint8_t I2C_S_7B_R = 0x86;
	static const uint8_t I2C_S_10B_W = 0x8A;
	static const uint8_t I2C_S_10B_R = 0x8E;
	static const uint8_t I2C_M_7B_W = 0xDA;
	static const uint8_t I2C_M_7B_R = 0xDE;
	static const uint8_t I2C_M_10B_W = 0xE2;
	static const uint8_t I2C_M_10B_R = 0xE6;
	/*
	//static const uint8_t I2C_RMW_AND
	//static const uint8_t I2C_RMW_OR = 0xC6;
	//static const uint8_t I2C_RMW_XOR = 0xCA;
	*/

	// ADC Operations
	static const uint8_t ADC_CH = 0x14;
	static const uint8_t ADC_GO = 0x02;
	static const uint8_t ADC_W_MUX = 0x50;
	static const uint8_t ADC_R_MUX = 0x51;
	static const uint8_t ADC_W_CURR = 0x60;
	static const uint8_t ADC_R_CURR = 0x61;
	static const uint8_t ADC_W_GAIN = 0x10;
	static const uint8_t ADC_R_GAIN = 0x11;
	static const uint8_t ADC_R_DATA = 0x21;
	static const uint8_t ADC_R_RAW = 0x31;
	static const uint8_t ADC_R_OFS = 0x41;

	// GPIO Operations
	static const uint8_t GPIO_CH = 0x02;
	static const uint8_t GPIO_W_DATAOUT = 0x10;
	static const uint8_t GPIO_R_DATAOUT = 0x11;
	static const uint8_t GPIO_R_DATAIN = 0x01;
	static const uint8_t GPIO_W_DIRECTION = 0x20;
	static const uint8_t GPIO_R_DIRECTION = 0x21;
	static const uint8_t GPIO_W_INTENABLE = 0x60;
	static const uint8_t GPIO_R_INTENABLE = 0x61;
	static const uint8_t GPIO_W_INTSEL = 0x30;
	static const uint8_t GPIO_R_INTSEL = 0x31;
	static const uint8_t GPIO_W_INTTRIG = 0x40;
	static const uint8_t GPIO_R_INTTRIG = 0x41;
	static const uint8_t GPIO_W_INTS = 0x70;
	static const uint8_t GPIO_R_INTS = 0x71;
	static const uint8_t GPIO_W_CLKSEL = 0x80;
	static const uint8_t GPIO_R_CLKSEL = 0x81;
	static const uint8_t GPIO_W_EDGESEL = 0x90;
	static const uint8_t GPIO_R_EDGESEL = 0x91;
	
	ScaApi()
	{
		srand(chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	
	inline uint8_t rn()
	{
		// return dist(eng);
		uint8_t n = (rand() % 232) + 2;
		if (n > 0x78 && n <= 0x7f) n = n+16;
		return n;
	}
	
	inline std::string getSequence()
	{
		// trim last newline
		sequence.erase(sequence.length()-1);
		return sequence;
	}
	
	// TODO: Handle FAILURE response
	inline std::vector<tuple<int,string>> deconstructAlfResponse(string alfResponse)
	{
		std::vector<tuple<int,string>> deconstruction;
		std::vector<string> splitAlfResponse = Utility::splitString(alfResponse, "\n");
		if (splitAlfResponse[0].find("success") != std::string::npos)
		{
			int currentPosition = 1; // Ignore first line (expect [success|failure])
			for (auto const& sequenceElement : sequenceConstruction)
			{
				const auto& [scaOperationType, scaOperationLength] = sequenceElement;
				int endCurrentPosition = currentPosition + scaOperationLength;
				std::string thisOperationAlfR;
				for (int i = currentPosition; i < endCurrentPosition; ++i)
				{
					thisOperationAlfR.append(splitAlfResponse[i]+"\n");
				}
				std::string responseData = getScaOperationValue(scaOperationType, scaOperationLength, thisOperationAlfR);
				deconstruction.emplace_back(std::make_tuple(scaOperationType, responseData));
				currentPosition = endCurrentPosition; // for the next time around the loop, start well (remeber off-by-one)
			}
		}
		else
		{
			std::tuple<int,std::string> fail = std::make_tuple(0x0, "ERROR");
			deconstruction.emplace_back(fail);
		}
		return deconstruction;
	}

	inline void add_CTRL_R_ID(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ADC_CH, trid, 1, CTRL_R_ID, 1));
		std::tuple<int, int> op = std::make_tuple(CTRL_R_ID, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_CTRL_W_CRB(uint32_t data, uint8_t trid = 0){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(0,trid,1,CTRL_W_CRB,data));
                std::tuple<int,int> op = std::make_tuple(CTRL_W_CRB, 1);
                sequenceConstruction.emplace_back(op);
        }
        inline void add_CTRL_W_CRC(uint32_t data, uint8_t trid = 0){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(0,trid,1,CTRL_W_CRC,data));
                std::tuple<int,int> op = std::make_tuple(CTRL_W_CRC, 1);
                sequenceConstruction.emplace_back(op);
        }
        inline void add_CTRL_W_CRD(uint32_t data,uint8_t trid = 0){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(0,trid,1,CTRL_W_CRD,data));
                std::tuple<int,int> op = std::make_tuple(CTRL_W_CRD, 1);
                sequenceConstruction.emplace_back(op);
        }
	inline void add_CTRL_R_CRB(uint8_t trid = 0){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(0,trid,1,CTRL_R_CRB));
                std::tuple<int,int> op = std::make_tuple(CTRL_R_CRB, 1);
                sequenceConstruction.emplace_back(op);
        }
        inline void add_CTRL_R_CRC(uint8_t trid = 0){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(0,trid,1,CTRL_R_CRC));
                std::tuple<int,int> op = std::make_tuple(CTRL_R_CRC, 1);
                sequenceConstruction.emplace_back(op);
        }
        inline void add_CTRL_R_CRD(uint8_t trid = 0 ){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(0,trid,1,CTRL_R_CRD));
                std::tuple<int,int> op = std::make_tuple(CTRL_R_CRD, 1);
                sequenceConstruction.emplace_back(op);
        }
                                                                                    
                                                                                                                                                                                                           
        inline void add_SC_RESET(){
                sequence.append("sc_reset\n");
        }
        inline void add_SVL_RESET(){
                sequence.append("svl_reset\n");
        }
        inline void add_SVL_CONNECT(){
                sequence.append("svl_connect\n");
                std::tuple<int, int> op = std::make_tuple(SCA_SVL_CONNECT, 1);
                sequenceConstruction.emplace_back(op);
        }
                                                                                                                                	
	inline void add_SCA_WAIT(int waitDuration)
	{
		sequence.append(to_string(waitDuration) + ",wait\n");
		std::tuple<int, int> op = std::make_tuple(SCA_WAIT, 1);
		sequenceConstruction.emplace_back(op);
	}

	/**
  	* Start specific protocol definitions
  	*
  	*/
	inline void add_I2C_W_CTRL(uint8_t ch, uint8_t val, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 4, I2C_W_CTRL, (val << 24)));
		std::tuple<int, int> op = std::make_tuple(I2C_W_CTRL, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_I2C_R_CTRL(uint8_t ch, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 0, I2C_R_CTRL));
		std::tuple<int, int> op = std::make_tuple(I2C_R_CTRL, 1);
		sequenceConstruction.emplace_back(op);
	}
	
	inline void add_I2C_R_STR(uint8_t ch, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 1, I2C_R_STR));
		std::tuple<int, int> op = std::make_tuple(I2C_R_STR, 1);
		sequenceConstruction.emplace_back(op);
	}
	
	inline void add_I2C_W_MSK(uint8_t ch, uint8_t val, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 4, I2C_W_MSK, (val << 24)));
		std::tuple<int, int> op = std::make_tuple(I2C_W_MSK, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_I2C_R_MSK(uint8_t ch, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 1, I2C_R_MSK));
		std::tuple<int, int> op = std::make_tuple(I2C_R_MSK, 1);
		sequenceConstruction.emplace_back(op);
	}
	
	inline void add_I2C_S_7B_W(uint8_t ch, uint8_t addr, uint8_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 4, I2C_S_7B_W, (((addr << 8) + data) << 16)));
		std::tuple<int, int> op = std::make_tuple(I2C_S_7B_W, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_I2C_S_7B_R(uint8_t ch, uint8_t addr, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 4, I2C_S_7B_R, (addr << 24)));
		std::tuple<int, int> op = std::make_tuple(I2C_S_7B_R, 1);
		sequenceConstruction.emplace_back(op);
	}
	
	inline void add_I2C_M_7B_W(uint8_t ch, uint8_t addr, uint8_t ctrlByte, uint32_t data0, uint32_t data1, uint32_t data2, uint32_t data3, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
	//	uint8_t trid = 0 = rn();
		// Have to set NBYTE in I2C CONTROL register
		uint8_t len = (ctrlByte >> 2) & 0x1f;
		int lines;
		if (len > 0)
		{
			lines = 3;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA0, data0));
		}
		if (len > 4)
		{
			lines = 4;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA1, data1));
		}
		if (len > 8)
		{
			lines = 5;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA2, data2));
		}
		if (len > 12)
		{
			lines = 6;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA3, data3));
		}
		// Initiates I2C write
		sequence.append(constructPacket(ch, ++trid, 4, I2C_M_7B_W, (addr << 24)));
	
		int x = I2C_M_7B_W;
		std::tuple<int, int> op = std::make_tuple(x, lines);
		sequenceConstruction.emplace_back(op);
	}

	inline void add_I2C_M_7B_W(uint8_t ch, uint8_t addr, uint8_t ctrlByte, uint32_t data0, uint32_t data1, uint32_t data2, uint32_t data3,bool set_ctrl=true, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
	//	uint8_t trid = 0 = rn();
		// Have to set NBYTE in I2C CONTROL register
		if(set_ctrl) sequence.append(constructPacket(ch, trid, 2, I2C_W_CTRL, (ctrlByte << 24)));                
		uint8_t len = (ctrlByte >> 2) & 0x1f;
		int lines;
		if (len > 0)
		{
			lines = 3;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA0, data0));
		}
		if (len > 4)
		{
			lines = 4;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA1, data1));
		}
		if (len > 8)
		{
			lines = 5;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA2, data2));
		}
		if (len > 12)
		{
			lines = 6;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_W_DATA3, data3));
		}
		// Initiates I2C write
		sequence.append(constructPacket(ch, ++trid, 4, I2C_M_7B_W, (addr << 24)));
	
		int x = I2C_M_7B_W;
		if(!set_ctrl) lines--;
		std::tuple<int, int> op = std::make_tuple(x, lines);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_I2C_M_7B_R(uint8_t ch, uint8_t addr, uint8_t ctrlByte, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
	//	uint8_t trid = 0 = rn();
		// Have to set NBYTE in I2C CONTROL register
		sequence.append(constructPacket(ch, trid, 2, I2C_W_CTRL, ctrlByte));
		sequence.append(constructPacket(ch, ++trid, 4, I2C_M_7B_R, addr));
		uint8_t len = (ctrlByte >> 2) & 0x1f;
		int lines = 2;
		if (len > 12)
		{
			lines++;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_R_DATA3));
		}
		if (len > 8)
		{
			lines++;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_R_DATA2));
		}
		if (len > 4)
		{
			lines++;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_R_DATA1));
		}
		if (len > 0)
		{
			lines++;
			sequence.append(constructPacket(ch, ++trid, 4, I2C_R_DATA0));
		}
		int x = I2C_M_7B_R;
		std::tuple<int, int> op = std::make_tuple(x, lines);
		sequenceConstruction.emplace_back(op);
	}
	
	inline void add_I2C_W_DATA0(uint8_t ch, uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ch, trid, 4, I2C_W_DATA0, data));
		std::tuple<int, int> op = std::make_tuple(I2C_W_DATA0, 1);
		sequenceConstruction.emplace_back(op);
	}
        inline void add_I2C_R_DATA0(uint8_t ch, uint8_t trid = 0){
                trid = trid ? trid : rn();
                sequence.append(constructPacket(ch, trid, 4, I2C_R_DATA0));
                std::tuple<int, int> op = std::make_tuple(I2C_R_DATA0, 1);
                sequenceConstruction.emplace_back(op);
        }
	                                                                                	
	inline void add_ADC_GO(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ADC_CH, trid, 4, ADC_GO, 0x01));
		std::tuple<int, int> op = std::make_tuple(ADC_GO, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_ADC_W_MUX(uint8_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ADC_CH, trid, 4, ADC_W_MUX, data));
		std::tuple<int, int> op = std::make_tuple(ADC_W_MUX, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_ADC_R_MUX(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ADC_CH, trid, 1, ADC_R_MUX));
		std::tuple<int, int> op = std::make_tuple(ADC_R_MUX, 1);
		sequenceConstruction.emplace_back(op);
	}
	
	inline void add_ADC_W_CURR(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ADC_CH, trid, 4, ADC_W_CURR, data));
		std::tuple<int, int> op = std::make_tuple(ADC_W_CURR, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_ADC_R_CURR(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
		sequence.append(constructPacket(ADC_CH, trid, 1, ADC_R_CURR));
		std::tuple<int, int> op = std::make_tuple(ADC_R_CURR, 1);
		sequenceConstruction.emplace_back(op);
	}
	
	
	/**
 	*  GPIO
 	*/
	inline void add_GPIO_TXN(uint8_t cmd, uint8_t len, uint32_t data = 0, uint8_t trid = 0)
	{
		sequence.append(constructPacket(GPIO_CH, trid, len, cmd, data));
		std::tuple<int, int> op = std::make_tuple(cmd, 1);
		sequenceConstruction.emplace_back(op);
	}
	inline void add_GPIO_W_DATAOUT(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_DATAOUT, 4, data, trid);
	}
	inline void add_GPIO_R_DATAOUT(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_DATAOUT, 1, 0, trid);
	}
	inline void add_GPIO_R_DATAIN(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_DATAIN, 1, 0, trid);
	}
	inline void add_GPIO_W_DIRECTION(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_DIRECTION, 4, data, trid);
	}
	inline void add_GPIO_R_DIRECTION(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_DIRECTION, 1, 0, trid);
	}
	inline void add_GPIO_W_INTENABLE(bool intenable, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_INTENABLE, 4, intenable, trid);
	}
	inline void add_GPIO_R_INTENABLE(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_INTENABLE, 1, 0, trid);
	}
	inline void add_GPIO_W_INTSEL(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_INTSEL, 4, data, trid);
	}
	inline void add_GPIO_R_INTSEL(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_INTSEL, 1, 0, trid);
	}
	inline void add_GPIO_W_INTTRIG(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_INTTRIG, 4, data, trid);
	}
	inline void add_GPIO_R_INTTRIG(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_INTTRIG, 1, 0, trid);
	}
	inline void add_GPIO_W_INTS(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_INTS, 4, data, trid);
	}
	inline void add_GPIO_R_INTS(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_INTS, 1, 0, trid);
	}
	inline void add_GPIO_W_CLKSEL(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_CLKSEL, 4, data, trid);
	}
	inline void add_GPIO_R_CLKSEL(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_CLKSEL, 1, 0, trid);
	}
	inline void add_GPIO_W_EDGESEL(uint32_t data, uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_W_EDGESEL, 4, data, trid);
	}
	inline void add_GPIO_R_EDGESEL(uint8_t trid = 0)
	{
		trid = trid ? trid : rn();
    	add_GPIO_TXN(GPIO_R_EDGESEL, 1, 0, trid);
	}
	
};
} // namespace

#endif // GBTSCAAPI_H
