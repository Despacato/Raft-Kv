set(RAFTCORE
    ${CMAKE_CURRENT_SOURCE_DIR}/kvServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Persister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raft.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raftRpcUtil.cpp
)

set(src_raftCore ${RAFTCORE} CACHE INTERNAL "Description of the variable")