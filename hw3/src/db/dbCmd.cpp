/****************************************************************************
  FileName     [ dbCmd.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include "util.h"
#include "dbCmd.h"
#include "dbJson.h"

// Global variable
DBJson dbjson;

bool initDbCmd()
{
   // TODO...
   if (!(cmdMgr->regCmd("DBAPpend", 4, new DBAppendCmd) &&
         cmdMgr->regCmd("DBAVerage", 4, new DBAveCmd) &&
         cmdMgr->regCmd("DBCount", 3, new DBCountCmd) &&
         cmdMgr->regCmd("DBMAx", 4, new DBMaxCmd) &&
         cmdMgr->regCmd("DBMIn", 4, new DBMinCmd) &&
         cmdMgr->regCmd("DBPrint", 3, new DBPrintCmd) &&
         cmdMgr->regCmd("DBRead", 3, new DBReadCmd) &&
         cmdMgr->regCmd("DBSOrt", 4, new DBSortCmd) &&
         cmdMgr->regCmd("DBSUm", 4, new DBSumCmd)))
   {
      cerr << "Registering \"init\" db commands fails... exiting" << endl;
      return false;
   }
   return true;
}

//----------------------------------------------------------------------
//    DBAPpend <(string key)><(int value)>
//----------------------------------------------------------------------
CmdExecStatus
DBAppendCmd::exec(const string &option)
{
   // TODO...
   // check option
   if (!dbjson)
   {
      cerr << "Error: DB is not created yet!!" << endl;
      return CMD_EXEC_ERROR;
   }
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;
   if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   // for (int i = 0; i != options.size(); i++)
   // cout << "options[" << i << "] " << options[i] << endl;

   else if (options.size() == 1)
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
   else if (options.size() == 2)
   {
      int value;
      if (!isValidVarName(options[0])) // if it is illegal key
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
      if (!myStr2Int(options[1], value)) // if it is illegal value
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      
      DBJsonElem jsonelem(options[0], value);
      
      if (!(dbjson.add(jsonelem))) // if the key already exist
      {
         cerr << "Error: Element with key \"" << options[0] << "\" already exists!!" << endl;
         return CMD_EXEC_ERROR;
      }
      else
      {
         dbjson.add(jsonelem);
         return CMD_EXEC_DONE;
      }
   }
   else
      return CmdExec::errorOption(CMD_OPT_EXTRA, options[2]);
}

void DBAppendCmd::usage(ostream &os) const
{
   os << "Usage: DBAPpend <(string key)><(int value)>" << endl;
}

void DBAppendCmd::help() const
{
   cout << setw(15) << left << "DBAPpend: "
        << "append an JSON element (key-value pair) to the end of DB" << endl;
}

//----------------------------------------------------------------------
//    DBAVerage
//----------------------------------------------------------------------
CmdExecStatus
DBAveCmd::exec(const string &option)
{
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   float a = dbjson.ave();
   if (isnan(a))
   {
      cerr << "Error: The average of the DB is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   ios_base::fmtflags origFlags = cout.flags();
   cout << "The average of the DB is " << fixed
        << setprecision(2) << a << ".\n";
   cout.flags(origFlags);

   return CMD_EXEC_DONE;
}

void DBAveCmd::usage(ostream &os) const
{
   os << "Usage: DBAVerage" << endl;
}

void DBAveCmd::help() const
{
   cout << setw(15) << left << "DBAVerage: "
        << "compute the average of the DB" << endl;
}

//----------------------------------------------------------------------
//    DBCount
//----------------------------------------------------------------------
CmdExecStatus
DBCountCmd::exec(const string &option)
{
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   size_t n = dbjson.size();
   if (n > 1)
      cout << "There are " << n << " JSON elements in DB." << endl;
   else if (n == 1)
      cout << "There is 1 JSON element in DB." << endl;
   else
      cout << "There is no JSON element in DB." << endl;

   return CMD_EXEC_DONE;
}

void DBCountCmd::usage(ostream &os) const
{
   os << "Usage: DBCount" << endl;
}

void DBCountCmd::help() const
{
   cout << setw(15) << left << "DBCount: "
        << "report the number of JSON elements in the DB" << endl;
}

//----------------------------------------------------------------------
//    DBMAx
//----------------------------------------------------------------------
CmdExecStatus
DBMaxCmd::exec(const string &option)
{
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   size_t maxI;
   int maxN = dbjson.max(maxI);
   if (maxN == INT_MIN)
   {
      cerr << "Error: The max JSON element is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   cout << "The max JSON element is { " << dbjson[maxI] << " }." << endl;

   return CMD_EXEC_DONE;
}

void DBMaxCmd::usage(ostream &os) const
{
   os << "Usage: DBMAx" << endl;
}

void DBMaxCmd::help() const
{
   cout << setw(15) << left << "DBMAx: "
        << "report the maximum JSON element" << endl;
}

//----------------------------------------------------------------------
//    DBMIn
//----------------------------------------------------------------------
CmdExecStatus
DBMinCmd::exec(const string &option)
{
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   size_t minI;
   int minN = dbjson.min(minI);
   if (minN == INT_MAX)
   {
      cerr << "Error: The min JSON element is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   cout << "The min JSON element is { " << dbjson[minI] << " }." << endl;

   return CMD_EXEC_DONE;
}

void DBMinCmd::usage(ostream &os) const
{
   os << "Usage: DBMIn" << endl;
}

void DBMinCmd::help() const
{
   cout << setw(15) << left << "DBMIn: "
        << "report the minimum JSON element" << endl;
}

//----------------------------------------------------------------------
//    DBPrint [(string key)]
//----------------------------------------------------------------------
CmdExecStatus
DBPrintCmd::exec(const string &option)
{
   // TODO...
   if (!dbjson)
   {
      cerr << "Error: DB is not created yet!!" << endl;
      return CMD_EXEC_ERROR;
   }

   bool option_is_all_space = true;
   for (int i = 0; i != option.size(); i++)
   {
      if (option[i] != ' ')
      {
         option_is_all_space = false;
         break;
      }
   }
   if (option_is_all_space)
   {
      cout << dbjson;
      cout << "Total JSON elements: " << dbjson.size() << endl;
      return CMD_EXEC_DONE;
   }
   else
   {
      string option_ = option;
      int firstspace = option_.find_first_of(' ');
      string findkey;
      string remainword;
      bool found = false;
      // parse the ket and remain
      if (firstspace == string::npos)
         findkey = option_;
      else
      {
         findkey = option_.substr(0, firstspace);                   // get the first word and find key
         remainword = option_.substr(firstspace, option_.length()); // the second and beyond
      }

      if (remainword.find_first_not_of(' ') == string::npos)
      { // if there is no remain word, then find the key
         int j;
         for (int i = 0; i != dbjson.size(); i++)
         {
            if (findkey == dbjson[i].key())
            {
               found = true;
               j = i;
            }
         }
         if (found)
         {
            cout << "{ " << dbjson[j] << " }" << endl;
            return CMD_EXEC_DONE;
         }
         else
         {
            cerr << "Error: No JSON element with key \"" << findkey << "\" is found." << endl;
            return CMD_EXEC_ERROR;
         }
      }
      else
      {
         return CmdExec::errorOption(CMD_OPT_EXTRA, remainword);
      }
   }
}

void DBPrintCmd::usage(ostream &os) const
{
   os << "DBPrint [(string key)]" << endl;
}

void DBPrintCmd::help() const
{
   cout << setw(15) << left << "DBPrint: "
        << "print the JSON element(s) in the DB" << endl;
}

//----------------------------------------------------------------------
//    DBRead <(string jsonFile)> [-Replace]
//----------------------------------------------------------------------
CmdExecStatus
DBReadCmd::exec(const string &option)
{
   // check option
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

   if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   bool doReplace = false;
   string fileName;
   for (size_t i = 0, n = options.size(); i < n; ++i)
   {
      if (myStrNCmp("-Replace", options[i], 2) == 0)
      {
         if (doReplace)
            return CmdExec::errorOption(CMD_OPT_EXTRA, options[i]);
         doReplace = true;
      }
      else
      {
         if (fileName.size())
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
         fileName = options[i];
      }
   }

   ifstream ifs(fileName.c_str());
   if (!ifs)
   {
      cerr << "Error: \"" << fileName << "\" does not exist!!" << endl;
      return CMD_EXEC_ERROR;
   }

   if (dbjson)
   {
      if (!doReplace)
      {
         cerr << "Error: DB exists. Use \"-Replace\" option for "
              << "replacement.\n";
         return CMD_EXEC_ERROR;
      }
      cout << "DB is replaced..." << endl;
      dbjson.reset();
   }
   //   if (!(ifs >> dbtbl)) return CMD_EXEC_ERROR;
   ifs >> dbjson;
   cout << "\"" << fileName << "\" was read in successfully." << endl;

   return CMD_EXEC_DONE;
}

void DBReadCmd::usage(ostream &os) const
{
   os << "Usage: DBRead <(string jsonFile)> [-Replace]" << endl;
}

void DBReadCmd::help() const
{
   cout << setw(15) << left << "DBRead: "
        << "read data from .json file" << endl;
}

//----------------------------------------------------------------------
//    DBSOrt <-Key | -Value>
//----------------------------------------------------------------------
CmdExecStatus
DBSortCmd::exec(const string &option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token, false))
      return CMD_EXEC_ERROR;

   if (myStrNCmp("-Key", token, 2) == 0)
      dbjson.sort(DBSortKey());
   else if (myStrNCmp("-Value", token, 2) == 0)
      dbjson.sort(DBSortValue());
   else
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);

   return CMD_EXEC_DONE;
}

void DBSortCmd::usage(ostream &os) const
{
   os << "Usage: DBSOrt <-Key | -Value>" << endl;
}

void DBSortCmd::help() const
{
   cout << setw(15) << left << "DBSOrt: "
        << "sort the JSON object by key or value" << endl;
}

//----------------------------------------------------------------------
//    DBSUm
//----------------------------------------------------------------------
CmdExecStatus
DBSumCmd::exec(const string &option)
{
   // check option
   if (!CmdExec::lexNoOption(option))
      return CMD_EXEC_ERROR;

   if (dbjson.empty())
   {
      cerr << "Error: The sum of the DB is nan." << endl;
      return CMD_EXEC_ERROR;
   }
   cout << "The sum of the DB is " << dbjson.sum() << "." << endl;

   return CMD_EXEC_DONE;
}

void DBSumCmd::usage(ostream &os) const
{
   os << "Usage: DBSUm" << endl;
}

void DBSumCmd::help() const
{
   cout << setw(15) << left << "DBSUm: "
        << "compute the summation of the DB" << endl;
}
