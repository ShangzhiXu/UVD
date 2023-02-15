//===- saber.cpp -- Source-sink bug checker------------------------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2023>  <Shangzhi Xu>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===-----------------------------------------------------------------------===//

/*
 // Saber: Software Bug Check.
 //
 // Author: Yulei Sui,
 */

#include "SABER/DoubleFreeChecker.h"
#include "SABER/FileChecker.h"
#include "SABER/LeakChecker.h"
#include "SABER/UseAfterFreeChecker.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "Util/Z3Expr.h"
#include "Util/PTAStat.h"
#include "Graphs/ICFG.h"
#include <iomanip>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "SVFIR/SVFModule.h"
#include "Util/SVFUtil.h"
#include "SVF-LLVM/BasicTypes.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "Util/CppUtil.h"
#include "SVFIR/SVFValue.h"
#include "SVFIR/PAGBuilderFromFile.h"
#include "SVF-LLVM/LLVMLoopAnalysis.h"
#include "Util/Options.h"
#include "SVF-LLVM/CHGBuilder.h"

using namespace llvm;
using namespace SVF;
using namespace std;
using namespace SVFUtil;
using namespace LLVMUtil;


int main(int argc, char ** argv)
{

    char **arg_value = new char*[argc];
    std::vector<std::string> moduleNameVec;
    moduleNameVec = OptionBase::parseOptions(
                        argc, argv, "Source-Sink Bug Detector", "[options] <input-bitcode...>"
                    );

    LLVMModuleSet* llvmModuleSet = LLVMModuleSet::getLLVMModuleSet();
    SVFModule* svfModule = llvmModuleSet->buildSVFModule(moduleNameVec);//create the module, init, create id for each val and each instruction and so on
    SVFIRBuilder builder(svfModule);
    SVFIR* pag = builder.build_icfg();// create pag(program assignment graph)

    std::unique_ptr<UseAfterFreeChecker> uvd = std::make_unique<UseAfterFreeChecker>();



    //uvd->runOnModule(pag);
    ICFG* icfg = pag->getICFG();
    NodeID nodeid = 1;
    ICFGNode* iNode = icfg->getICFGNode(nodeid);
    FIFOWorkList<const ICFGNode*> worklist;
    string string1 = iNode->toString();
    Set<const ICFGNode*> visited;
    worklist.push(iNode);

    /// Traverse along VFG
    while (!worklist.empty())
    {
        const ICFGNode* vNode = worklist.pop();
        for (ICFGNode::const_iterator it = vNode->OutEdgeBegin(), eit = vNode->OutEdgeEnd(); it != eit; ++it)
        {
            ICFGEdge* edge = *it;
            ICFGNode* succNode = edge->getDstNode();
            if (visited.find(succNode) == visited.end())
            {
                visited.insert(succNode);
                if (IntraICFGNode* intraNode = SVFUtil::dyn_cast<IntraICFGNode>(succNode)){
                    const SVFInstruction* svfstmts = const_cast<SVFInstruction*>(intraNode->getInst());
                    const Instruction* inst = static_cast<const Instruction*>(
                        llvmModuleSet->getLLVMInst(svfstmts));
                    //here, we successfully get the instruction from the ICFG nodes
                    std::string str;
                    llvm::raw_string_ostream ss(str);
                    inst->print(ss);
                    ss.flush();
                    cout << str << endl;
                }
                worklist.push(succNode);
            }
        }
    }


    icfg->dump("icfg_initial");
    delete[] arg_value;

    return 0;

}
