//
// Created by shangzhixu on 2023/1/18.
//

#ifndef SVF_USEAFTERFREECHECKER_H
#define SVF_USEAFTERFREECHECKER_H
#include <iostream>

using namespace std;
#include "SABER/SrcSnkDDA.h"
#include "SABER/SaberCheckerAPI.h"

namespace SVF
{

/*!
 * Static Memory Leak Detector
 */
class UseAfterFreeChecker : public SrcSnkDDA
{

public:
    typedef Map<const SVFGNode*,const CallICFGNode*> SVFGNodeToCSIDMap;
    typedef FIFOWorkList<const CallICFGNode*> CSWorkList;
    typedef ProgSlice::VFWorkList WorkList;
    typedef NodeBS SVFGNodeBS;
    enum LEAK_TYPE
    {
        NEVER_FREE_LEAK,
        CONTEXT_LEAK,
        PATH_LEAK,
        GLOBAL_LEAK
    };

    /// Constructor
    UseAfterFreeChecker()
    {
    }
    /// Destructor
    virtual ~UseAfterFreeChecker()
    {
    }

    virtual void analyze(SVFModule* module);
    /// We start from here
    virtual bool runOnModule(SVFIR* pag)
    {
        /// start analysis
        SrcSnkDDA::analyze(pag->getModule());
        return false;
    }

    /// Initialize sources and sinks
    //@{
    /// Initialize sources and sinks
    virtual void initSrcs() override;
    virtual void initSnks() override;
    /// Whether the function is a heap allocator/reallocator (allocate memory)
    virtual inline bool isSourceLikeFun(const SVFFunction* fun) override
    {
        return SaberCheckerAPI::getCheckerAPI()->isMemAlloc(fun);
    }
    /// Whether the function is a heap deallocator (free/release memory)
    virtual inline bool isSinkLikeFun(const SVFFunction* fun) override
    {
        return SaberCheckerAPI::getCheckerAPI()->isMemDealloc(fun);
    }
    //@}

protected:
    /// Report leaks
    //@{
    virtual void reportBug(ProgSlice* slice) override;
    void reportNeverFree(const SVFGNode* src);
    void reportPartialLeak(const SVFGNode* src);
    //@}

    /// Validate test cases for regression test purpose
    void testsValidation(const ProgSlice* slice);
    void validateSuccessTests(const SVFGNode* source, const SVFFunction* fun);
    void validateExpectedFailureTests(const SVFGNode* source, const SVFFunction* fun);

    /// Record a source to its callsite
    //@{
    inline void addSrcToCSID(const SVFGNode* src, const CallICFGNode* cs)
    {
        srcToCSIDMap[src] = cs;
    }
    inline const CallICFGNode* getSrcCSID(const SVFGNode* src)
    {
        SVFGNodeToCSIDMap::iterator it =srcToCSIDMap.find(src);
        assert(it!=srcToCSIDMap.end() && "source node not at a callsite??");
        return it->second;
    }
    //@}
private:
    SVFGNodeToCSIDMap srcToCSIDMap;
};

} // End namespace SVF
#endif /* UAFchecker_H_ */
