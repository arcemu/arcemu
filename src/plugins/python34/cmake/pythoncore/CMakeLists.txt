PROJECT( pythoncore C )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( pythoncore_headers
    ../../src/Include/abstract.h
    ../../src/Include/accu.h
    ../../src/Include/asdl.h
    ../../src/Include/ast.h
    ../../src/Include/bitset.h
    ../../src/Include/boolobject.h
    ../../src/Include/bytes_methods.h
    ../../src/Include/bytearrayobject.h
    ../../src/Include/bytesobject.h
    ../../src/Include/cellobject.h
    ../../src/Include/ceval.h
    ../../src/Include/classobject.h
    ../../src/Include/code.h
    ../../src/Include/codecs.h
    ../../src/Include/compile.h
    ../../src/Include/complexobject.h
    ../../src/Include/datetime.h
    ../../src/Include/descrobject.h
    ../../src/Include/dictobject.h
    ../../src/Include/dynamic_annotations.h
    ../../src/Include/enumobject.h
    ../../src/Include/errcode.h
    ../../src/Include/eval.h
    ../../src/Include/fileobject.h
    ../../src/Include/fileutils.h
    ../../src/Include/floatobject.h
    ../../src/Include/frameobject.h
    ../../src/Include/funcobject.h
    ../../src/Include/genobject.h
    ../../src/Include/graminit.h
    ../../src/Include/grammar.h
    ../../src/Include/import.h
    ../../src/Include/intrcheck.h
    ../../src/Include/iterobject.h
    ../../src/Include/listobject.h
    ../../src/Include/longintrepr.h
    ../../src/Include/longobject.h
    ../../src/Include/marshal.h
    ../../src/Include/memoryobject.h
    ../../src/Include/metagrammar.h
    ../../src/Include/methodobject.h
    ../../src/Include/modsupport.h
    ../../src/Include/moduleobject.h
    ../../src/Include/namespaceobject.h
    ../../src/Include/node.h
    ../../src/Include/object.h
    ../../src/Include/objimpl.h
    ../../src/Include/opcode.h
    ../../src/Include/osdefs.h
    ../../src/Include/parsetok.h
    ../../src/Include/patchlevel.h
    ../../src/Include/pgen.h
    ../../src/Include/pgenheaders.h
    ../../src/Include/pyhash.h
    ../../src/Include/py_curses.h
    ../../src/Include/pyarena.h
    ../../src/Include/pycapsule.h
    ../../src/Include/pyctype.h
    ../../src/Include/pydebug.h
    ../../src/Include/pyerrors.h
    ../../src/Include/pyexpat.h
    ../../src/Include/pyfpe.h
    ../../src/Include/pygetopt.h
    ../../src/Include/pymath.h
    ../../src/Include/pytime.h
    ../../src/Include/pymacro.h
    ../../src/Include/pymem.h
    ../../src/Include/pyport.h
    ../../src/Include/pystate.h
    ../../src/Include/pystrcmp.h
    ../../src/Include/pystrtod.h
    ../../src/Include/dtoa.h
    ../../src/Include/Python-ast.h
    ../../src/Include/Python.h
    ../../src/Include/pythonrun.h
    ../../src/Include/pythread.h
    ../../src/Include/rangeobject.h
    ../../src/Include/setobject.h
    ../../src/Include/sliceobject.h
    ../../src/Include/structmember.h
    ../../src/Include/structseq.h
    ../../src/Include/symtable.h
    ../../src/Include/sysmodule.h
    ../../src/Include/token.h
    ../../src/Include/traceback.h
    ../../src/Include/tupleobject.h
    ../../src/Include/ucnhash.h
    ../../src/Include/unicodeobject.h
    ../../src/Include/weakrefobject.h
    ../../src/Modules/_math.h
    ../../src/Modules/hashtable.h
    ../../src/Modules/rotatingtree.h
    ../../src/Modules/sre.h
    ../../src/Modules/sre_constants.h
    ../../src/Modules/sre_lib.h
    ../../src/Modules/_io/_iomodule.h
    ../../src/Modules/cjkcodecs/alg_jisx0201.h
    ../../src/Modules/cjkcodecs/cjkcodecs.h
    ../../src/Modules/cjkcodecs/emu_jisx0213_2000.h
    ../../src/Modules/cjkcodecs/mappings_cn.h
    ../../src/Modules/cjkcodecs/mappings_hk.h
    ../../src/Modules/cjkcodecs/mappings_jisx0213_pair.h
    ../../src/Modules/cjkcodecs/mappings_jp.h
    ../../src/Modules/cjkcodecs/mappings_kr.h
    ../../src/Modules/cjkcodecs/mappings_tw.h
    ../../src/Modules/cjkcodecs/multibytecodec.h
    ../../src/Objects/stringlib/count.h
    ../../src/Objects/stringlib/fastsearch.h
    ../../src/Objects/stringlib/find.h
    ../../src/Objects/stringlib/partition.h
    ../../src/Objects/stringlib/replace.h
    ../../src/Objects/stringlib/split.h
    ../../src/Objects/unicodetype_db.h
    ../../src/Parser/parser.h
    ../../src/Parser/tokenizer.h
    ../../src/Python/ceval_gil.h
    ../../src/Python/condvar.h
    ../../src/Python/importdl.h
    ../../src/Python/thread_nt.h )

IF(CMAKE_GENERATOR MATCHES Visual*)
    SET( pythoncore_headers
		${pythoncore_headers}
		../../src/platform/Windows/errmap.h
		../../src/platform/Windows/pyconfig.h )
ENDIF()


SET( pythoncore_sources
    ../../src/Modules/getbuildinfo.c
    ../../src/Modules/_bisectmodule.c
    ../../src/Modules/_codecsmodule.c
    ../../src/Modules/_collectionsmodule.c
    ../../src/Modules/_functoolsmodule.c
    ../../src/Modules/_heapqmodule.c
    ../../src/Modules/_json.c
    ../../src/Modules/_localemodule.c
    ../../src/Modules/_lsprof.c
    ../../src/Modules/_math.c
    ../../src/Modules/_pickle.c
    ../../src/Modules/_randommodule.c
    ../../src/Modules/_sre.c
    ../../src/Modules/_stat.c
    ../../src/Modules/_struct.c
    ../../src/Modules/_weakref.c
    ../../src/Modules/arraymodule.c
    ../../src/Modules/atexitmodule.c
    ../../src/Modules/binascii.c
    ../../src/Modules/cmathmodule.c
    ../../src/Modules/_datetimemodule.c
    ../../src/Modules/errnomodule.c
    ../../src/Modules/faulthandler.c
    ../../src/Modules/gcmodule.c
    ../../src/Modules/hashtable.c
    ../../src/Modules/itertoolsmodule.c
    ../../src/Modules/main.c
    ../../src/Modules/mathmodule.c
    ../../src/Modules/md5module.c
    ../../src/Modules/mmapmodule.c
    ../../src/Modules/_opcode.c
    ../../src/Modules/_operator.c
    ../../src/Modules/parsermodule.c
    ../../src/Modules/posixmodule.c
    ../../src/Modules/rotatingtree.c
    ../../src/Modules/sha1module.c
    ../../src/Modules/sha256module.c
    ../../src/Modules/sha512module.c
    ../../src/Modules/signalmodule.c
    ../../src/Modules/symtablemodule.c
    ../../src/Modules/_threadmodule.c
    ../../src/Modules/_tracemalloc.c
    ../../src/Modules/timemodule.c
    ../../src/Modules/xxsubtype.c
    ../../src/Modules/_io/fileio.c
    ../../src/Modules/_io/bytesio.c
    ../../src/Modules/_io/stringio.c
    ../../src/Modules/_io/bufferedio.c
    ../../src/Modules/_io/iobase.c
    ../../src/Modules/_io/textio.c
    ../../src/Modules/_io/_iomodule.c
    ../../src/Modules/cjkcodecs/_codecs_cn.c
    ../../src/Modules/cjkcodecs/_codecs_hk.c
    ../../src/Modules/cjkcodecs/_codecs_iso2022.c
    ../../src/Modules/cjkcodecs/_codecs_jp.c
    ../../src/Modules/cjkcodecs/_codecs_kr.c
    ../../src/Modules/cjkcodecs/_codecs_tw.c
    ../../src/Modules/cjkcodecs/multibytecodec.c
    ../../src/Objects/abstract.c
    ../../src/Objects/accu.c
    ../../src/Objects/boolobject.c
    ../../src/Objects/bytes_methods.c
    ../../src/Objects/bytearrayobject.c
    ../../src/Objects/bytesobject.c
    ../../src/Objects/capsule.c
    ../../src/Objects/cellobject.c
    ../../src/Objects/classobject.c
    ../../src/Objects/codeobject.c
    ../../src/Objects/complexobject.c
    ../../src/Objects/descrobject.c
    ../../src/Objects/dictobject.c
    ../../src/Objects/enumobject.c
    ../../src/Objects/exceptions.c
    ../../src/Objects/fileobject.c
    ../../src/Objects/floatobject.c
    ../../src/Objects/frameobject.c
    ../../src/Objects/funcobject.c
    ../../src/Objects/genobject.c
    ../../src/Objects/iterobject.c
    ../../src/Objects/listobject.c
    ../../src/Objects/longobject.c
    ../../src/Objects/memoryobject.c
    ../../src/Objects/methodobject.c
    ../../src/Objects/moduleobject.c
    ../../src/Objects/namespaceobject.c
    ../../src/Objects/object.c
    ../../src/Objects/obmalloc.c
    ../../src/Objects/rangeobject.c
    ../../src/Objects/setobject.c
    ../../src/Objects/sliceobject.c
    ../../src/Objects/structseq.c
    ../../src/Objects/tupleobject.c
    ../../src/Objects/typeobject.c
    ../../src/Objects/unicodectype.c
    ../../src/Objects/unicodeobject.c
    ../../src/Objects/weakrefobject.c
    ../../src/Parser/acceler.c
    ../../src/Parser/bitset.c
    ../../src/Parser/firstsets.c
    ../../src/Parser/grammar.c
    ../../src/Parser/grammar1.c
    ../../src/Parser/listnode.c
    ../../src/Parser/metagrammar.c
    ../../src/Parser/myreadline.c
    ../../src/Parser/node.c
    ../../src/Parser/parser.c
    ../../src/Parser/parsetok.c
    ../../src/Parser/tokenizer.c
    ../../src/Python/pyhash.c
    ../../src/Python/random.c
    ../../src/Python/_warnings.c
    ../../src/Python/asdl.c
    ../../src/Python/ast.c
    ../../src/Python/bltinmodule.c
    ../../src/Python/ceval.c
    ../../src/Python/codecs.c
    ../../src/Python/compile.c
    ../../src/Python/dynamic_annotations.c
    ../../src/Python/errors.c
    ../../src/Python/fileutils.c
    ../../src/Python/formatter_unicode.c
    ../../src/Python/frozen.c
    ../../src/Python/future.c
    ../../src/Python/getargs.c
    ../../src/Python/getcompiler.c
    ../../src/Python/getcopyright.c
    ../../src/Python/getopt.c
    ../../src/Python/getplatform.c
    ../../src/Python/getversion.c
    ../../src/Python/graminit.c
    ../../src/Python/import.c
    ../../src/Python/importdl.c
    ../../src/Python/marshal.c
    ../../src/Python/modsupport.c
    ../../src/Python/mysnprintf.c
    ../../src/Python/mystrtoul.c
    ../../src/Python/peephole.c
    ../../src/Python/pyarena.c
    ../../src/Python/pyctype.c
    ../../src/Python/pyfpe.c
    ../../src/Python/pymath.c
    ../../src/Python/pytime.c
    ../../src/Python/pystate.c
    ../../src/Python/pystrcmp.c
    ../../src/Python/pystrtod.c
    ../../src/Python/dtoa.c
    ../../src/Python/Python-ast.c
    ../../src/Python/pythonrun.c
    ../../src/Python/structmember.c
    ../../src/Python/symtable.c
    ../../src/Python/sysmodule.c
    ../../src/Python/thread.c
    ../../src/Python/traceback.c )

IF(CMAKE_GENERATOR MATCHES Visual*)
    SET( pythoncore_sources
	${pythoncore_sources}
	../../src/platform/Windows/winreg.c
    ../../src/platform/Windows/config.c
    ../../src/platform/Windows/dl_nt.c
    ../../src/platform/Windows/getpathp.c
    ../../src/platform/Windows/msvcrtmodule.c
    ../../src/Modules/_winapi.c
    ../../src/Python/dynload_win.c)
ELSE()
    SET( pythoncore_sources
    ${pythoncore_sources}
    ../../src/platform/Linux/config.c
    ../../src/Modules/getpath.c
    ../../src/Modules/pwdmodule.c
    ../../src/Modules/_posixsubprocess.c
    ../../src/Modules/selectmodule.c
    ../../src/Python/dynload_shlib.c )
ENDIF()

IF(CMAKE_GENERATOR MATCHES Visual*)
    INCLUDE_DIRECTORIES( ../../src/platform/Windows )
ELSE()
    INCLUDE_DIRECTORIES( ../../src/platform/Linux )
ENDIF()

INCLUDE_DIRECTORIES( ../../src/Include )

ADD_DEFINITIONS( -D_USRDLL -DPy_BUILD_CORE -DPy_ENABLE_SHARED -DSOABI=\"cpython-34m\" -DPREFIX=\".\" -DEXEC_PREFIX=\".\" -DVERSION=\"3.4\" -DVPATH=\"\")

IF(CMAKE_GENERATOR MATCHES Visual*)
    ADD_DEFINITIONS( -DWIN32 )
ENDIF()

ADD_LIBRARY( python34 SHARED ${pythoncore_sources} ${pythoncore_headers} )
INSTALL( TARGETS python34 DESTINATION ${INSTALL_DESTINATION} )
