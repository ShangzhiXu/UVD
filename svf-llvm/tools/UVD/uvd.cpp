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

//#include "SABER/UseAfterFreeChecker.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "Util/PTAStat.h"
#include "Graphs/ICFG.h"
#include <iomanip>
#include "SVFIR/SVFModule.h"
#include "SVF-LLVM/BasicTypes.h"
#include "SVFIR/SVFValue.h"
#include "SVFIR/PAGBuilderFromFile.h"
#include "WPA/Andersen.h"
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
    SVFIR* pag = builder.build();// create pag(program assignment graph)

    //UseAfterFreeChecker uvd = UseAfterFreeChecker(svfModule);



    //uvd->runOnModule(pag);
    ICFG* icfg = pag->getICFG();

    Andersen* ander = AndersenWaveDiff::createAndersenWaveDiff(pag);

    ICFGNode* iNode;

    FIFOWorkList<const ICFGNode*> worklist;

    auto srcsnkNodeSet = svfModule->getSrcSnkNodeSet();

    builder.setState(SVF::backward);
    //backwardanalysis
    for (auto nodeid = srcsnkNodeSet.begin(); nodeid != srcsnkNodeSet.end(); nodeid++){

        iNode = icfg->getICFGNode(*nodeid);

        string string1 = iNode->toString();

        worklist.push(iNode);
        /// Traverse along ICFG backwardly
        while (!worklist.empty())
        {
            const ICFGNode* vNode = worklist.pop();
            for (ICFGNode::const_iterator it = vNode->InEdgeBegin(), eit = vNode->InEdgeEnd(); it != eit; ++it)
            {
                ICFGEdge* edge = *it;

                ICFGNode* preNode = edge->getSrcNode();

                if (IntraICFGNode* intraNode = SVFUtil::dyn_cast<IntraICFGNode>(preNode)){

                    SVFInstruction* svfstmts = (SVFInstruction*)(intraNode->getInst());

                    Instruction* inst = (Instruction*)(
                            llvmModuleSet->getLLVMInst(svfstmts));
                    //here, we successfully get the instruction from the ICFG nodes
                    builder.visit(const_cast<Instruction&>(*inst));
                    std::string str;

                    llvm::raw_string_ostream ss(str);

                    inst->print(ss);

                    ss.flush();

                    cout << str << endl;
                }

                worklist.push(preNode);
            }
        }

        cout << endl;
    }
    builder.setState(SVF::forward);
    //forward analysis
    for (auto nodeid = srcsnkNodeSet.begin(); nodeid != srcsnkNodeSet.end(); nodeid++){

        iNode = icfg->getICFGNode(*nodeid);

        string string1 = iNode->toString();

        worklist.push(iNode);
        /// Traverse along ICFG backwardly
        while (!worklist.empty())
        {
            const ICFGNode* vNode = worklist.pop();
            for (ICFGNode::const_iterator it = vNode->OutEdgeBegin(), eit = vNode->OutEdgeEnd(); it != eit; ++it)
            {
                ICFGEdge* edge = *it;

                ICFGNode* succNode = edge->getDstNode();

                if (IntraICFGNode* intraNode = SVFUtil::dyn_cast<IntraICFGNode>(succNode)){

                    SVFInstruction* svfstmts = (SVFInstruction*)(intraNode->getInst());

                    Instruction* inst = (Instruction*)(
                        llvmModuleSet->getLLVMInst(svfstmts));
                    //here, we successfully get the instruction from the ICFG nodes
                    builder.visit(const_cast<Instruction&>(*inst));
                    std::string str;

                    llvm::raw_string_ostream ss(str);

                    inst->print(ss);

                    ss.flush();

                    cout << str << endl;
                }

                worklist.push(succNode);
            }
        }

        cout << endl;
    }

    icfg->dump("icfg_initial");
    delete[] arg_value;

    return 0;

}
