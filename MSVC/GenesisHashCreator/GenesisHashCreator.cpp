// GenesisHashCreator.cpp : Defines the entry point for the console application.
//
#include "..\..\src\main.h"
#include "..\..\src\util.h"
#include "..\..\src\chain.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
   fPrintToConsole = true;
   printf("Enter your line of text you wish to use to make the genesis block\nOr just hit return to test your changes:\n");
   std::string line;
   std::getline(std::cin, line);
   if (line.empty())
      printf("nothing entered, will use compiled string, time, and nonce values\n");
   else
      printf("will use this: %s\n", line.data());
   
   
   // Genesis block
   uint256 hashGenesisBlock;
   hashGenesisBlock = uint256("82af5ce8d2833d883f8bf59899f8dda13fb64fcd2519d3e84ab988057b7a460e");
   const char* pszTimestamp = "It's a wonderful world :)";
   CTransaction txNew;
   txNew.vin.resize(1);
   txNew.vout.resize(1);
   if (line.empty())
      txNew.vin[0].scriptSig = CScript() << 486604799 << nChainWork << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
   else
      txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>(line.data(), line.data() + line.length());
	string str1 = HexStr(txNew.vin[0].scriptSig).c_str();

   txNew.vout[0].nValue = 15 * COIN;
   txNew.vout[0].scriptPubKey = CScript() << ParseHex("04af5678523465762356237de78657823562346528365327952369ee876782346524735234fa864109630910301ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
   CBlock block;
   block.vtx.push_back(txNew);
   block.hashPrevBlock = 0;
   block.hashMerkleRoot = block.BuildMerkleTree();
   block.nVersion = 1;
   block.nBits    = 0x1e0ffff0;
   if (!line.empty())
      block.nTime = GetTime();
   else
      block.nTime = 1393012788;
   block.nNonce = 2089541887;

   uint256 hash = block.GetHash();
   
   static CBigNum bnProofOfWorkLimit(~uint256(0) >> 4);
   // If genesis block hash does not match, then generate new genesis hash.
   if (block.GetHash() != hashGenesisBlock)
   {
      printf("Searching for genesis block...\n");
      // This will figure out a valid hash and Nonce if you're
      // creating a different genesis block:
      uint256 hashTarget = CBigNum().SetCompact(block.nBits).getuint256();
       uint256 thash;
      char scratchpad[SCRYPT_SCRATCHPAD_SIZE];

      loop
      {
         scrypt_1024_1_1_256_sp(BEGIN(block.nVersion), BEGIN(thash), scratchpad);
         if (thash <= hashTarget) {
            printf("Found one!!!\n");
            break;
         }
         if ((block.nNonce & 0xFFF) == 0)
         {
           // printf("nonce %08X: hash = %s (target = %s)\n", block.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
         }
         ++block.nNonce;
         if (block.nNonce == 0)
         {
            //printf("NONCE WRAPPED, incrementing time\n");
            ++block.nTime;
         }
      }
      printf("Here are the new lines of code:\n");
      printf("const char* pszTimestamp = \"%s\";\n",line.data());
      printf("hashGenesisBlock = uint256(\"%s\");\n", block.GetHash().ToString().c_str());
      printf("merkle root = %s\n", block.hashMerkleRoot.ToString().c_str());
      printf("block.nTime = %u; \n", block.nTime);
      printf("block.nNonce = %u; \n", block.nNonce);
      printf("\n\nHere is the block:\n");
      block.print();
   }
   else {
      printf("It all looks good!\n\n");
      printf("const char* pszTimestamp = \"%s\";\n", pszTimestamp);
      printf("merkle root = %s\n", block.hashMerkleRoot.ToString().c_str());
      printf("block.nTime = %u \n", block.nTime);
      printf("block.nNonce = %u \n", block.nNonce);
      printf("block.GetHash = %s\n", block.GetHash().ToString().c_str());
      printf("\n\nHere is the block:\n");
      block.print();
   }

   printf("Hit return to end this\n");
   std::getline(std::cin, line);
   return 0;
}

