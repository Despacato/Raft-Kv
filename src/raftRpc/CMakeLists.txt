set(RAFTRPC_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/kvServerRPC.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/raftRPC.pb.cc
)

set(src_raftRpc ${RAFTRPC_LIST} CACHE INTERNAL "Description of the variable")