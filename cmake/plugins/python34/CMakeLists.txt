PROJECT( pythoncore C )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
SET( prefix ${ROOT_PATH}/src/plugins/python34/src )
SET( pythoncore_headers
    ${prefix}/Include/abstract.h
    ${prefix}/Include/accu.h
    ${prefix}/Include/asdl.h
    ${prefix}/Include/ast.h
    ${prefix}/Include/bitset.h
    ${prefix}/Include/boolobject.h
    ${prefix}/Include/bytes_methods.h
    ${prefix}/Include/bytearrayobject.h
    ${prefix}/Include/bytesobject.h
    ${prefix}/Include/cellobject.h
    ${prefix}/Include/ceval.h
    ${prefix}/Include/classobject.h
    ${prefix}/Include/code.h
    ${prefix}/Include/codecs.h
    ${prefix}/Include/compile.h
    ${prefix}/Include/complexobject.h
    ${prefix}/Include/datetime.h
    ${prefix}/Include/descrobject.h
    ${prefix}/Include/dictobject.h
    ${prefix}/Include/dynamic_annotations.h
    ${prefix}/Include/enumobject.h
    ${prefix}/Include/errcode.h
    ${prefix}/Include/eval.h
    ${prefix}/Include/fileobject.h
    ${prefix}/Include/fileutils.h
    ${prefix}/Include/floatobject.h
    ${prefix}/Include/frameobject.h
    ${prefix}/Include/funcobject.h
    ${prefix}/Include/genobject.h
    ${prefix}/Include/graminit.h
    ${prefix}/Include/grammar.h
    ${prefix}/Include/import.h
    ${prefix}/Include/intrcheck.h
    ${prefix}/Include/iterobject.h
    ${prefix}/Include/listobject.h
    ${prefix}/Include/longintrepr.h
    ${prefix}/Include/longobject.h
    ${prefix}/Include/marshal.h
    ${prefix}/Include/memoryobject.h
    ${prefix}/Include/metagrammar.h
    ${prefix}/Include/methodobject.h
    ${prefix}/Include/modsupport.h
    ${prefix}/Include/moduleobject.h
    ${prefix}/Include/namespaceobject.h
    ${prefix}/Include/node.h
    ${prefix}/Include/object.h
    ${prefix}/Include/objimpl.h
    ${prefix}/Include/opcode.h
    ${prefix}/Include/osdefs.h
    ${prefix}/Include/parsetok.h
    ${prefix}/Include/patchlevel.h
    ${prefix}/Include/pgen.h
    ${prefix}/Include/pgenheaders.h
    ${prefix}/Include/pyhash.h
    ${prefix}/Include/py_curses.h
    ${prefix}/Include/pyarena.h
    ${prefix}/Include/pycapsule.h
    ${prefix}/Include/pyctype.h
    ${prefix}/Include/pydebug.h
    ${prefix}/Include/pyerrors.h
    ${prefix}/Include/pyexpat.h
    ${prefix}/Include/pyfpe.h
    ${prefix}/Include/pygetopt.h
    ${prefix}/Include/pymath.h
    ${prefix}/Include/pytime.h
    ${prefix}/Include/pymacro.h
    ${prefix}/Include/pymem.h
    ${prefix}/Include/pyport.h
    ${prefix}/Include/pystate.h
    ${prefix}/Include/pystrcmp.h
    ${prefix}/Include/pystrtod.h
    ${prefix}/Include/dtoa.h
    ${prefix}/Include/Python-ast.h
    ${prefix}/Include/Python.h
    ${prefix}/Include/pythonrun.h
    ${prefix}/Include/pythread.h
    ${prefix}/Include/rangeobject.h
    ${prefix}/Include/setobject.h
    ${prefix}/Include/sliceobject.h
    ${prefix}/Include/structmember.h
    ${prefix}/Include/structseq.h
    ${prefix}/Include/symtable.h
    ${prefix}/Include/sysmodule.h
    ${prefix}/Include/token.h
    ${prefix}/Include/traceback.h
    ${prefix}/Include/tupleobject.h
    ${prefix}/Include/ucnhash.h
    ${prefix}/Include/unicodeobject.h
    ${prefix}/Include/weakrefobject.h
    ${prefix}/Modules/_math.h
    ${prefix}/Modules/hashtable.h
    ${prefix}/Modules/rotatingtree.h
    ${prefix}/Modules/sre.h
    ${prefix}/Modules/sre_constants.h
    ${prefix}/Modules/sre_lib.h
    ${prefix}/Modules/_io/_iomodule.h
    ${prefix}/Modules/cjkcodecs/alg_jisx0201.h
    ${prefix}/Modules/cjkcodecs/cjkcodecs.h
    ${prefix}/Modules/cjkcodecs/emu_jisx0213_2000.h
    ${prefix}/Modules/cjkcodecs/mappings_cn.h
    ${prefix}/Modules/cjkcodecs/mappings_hk.h
    ${prefix}/Modules/cjkcodecs/mappings_jisx0213_pair.h
    ${prefix}/Modules/cjkcodecs/mappings_jp.h
    ${prefix}/Modules/cjkcodecs/mappings_kr.h
    ${prefix}/Modules/cjkcodecs/mappings_tw.h
    ${prefix}/Modules/cjkcodecs/multibytecodec.h
    ${prefix}/Objects/stringlib/count.h
    ${prefix}/Objects/stringlib/fastsearch.h
    ${prefix}/Objects/stringlib/find.h
    ${prefix}/Objects/stringlib/partition.h
    ${prefix}/Objects/stringlib/replace.h
    ${prefix}/Objects/stringlib/split.h
    ${prefix}/Objects/unicodetype_db.h
    ${prefix}/Parser/parser.h
    ${prefix}/Parser/tokenizer.h
    ${prefix}/Python/ceval_gil.h
    ${prefix}/Python/condvar.h
    ${prefix}/Python/importdl.h
    ${prefix}/Python/thread_nt.h )

IF(CMAKE_GENERATOR MATCHES Visual*)
    SET( pythoncore_headers
		${pythoncore_headers}
		${prefix}/platform/Windows/errmap.h
		${prefix}/platform/Windows/pyconfig.h )
ENDIF()


SET( pythoncore_sources
    ${prefix}/Modules/getbuildinfo.c
    ${prefix}/Modules/_bisectmodule.c
    ${prefix}/Modules/_codecsmodule.c
    ${prefix}/Modules/_collectionsmodule.c
    ${prefix}/Modules/_functoolsmodule.c
    ${prefix}/Modules/_heapqmodule.c
    ${prefix}/Modules/_json.c
    ${prefix}/Modules/_localemodule.c
    ${prefix}/Modules/_lsprof.c
    ${prefix}/Modules/_math.c
    ${prefix}/Modules/_pickle.c
    ${prefix}/Modules/_randommodule.c
    ${prefix}/Modules/_sre.c
    ${prefix}/Modules/_stat.c
    ${prefix}/Modules/_struct.c
    ${prefix}/Modules/_weakref.c
    ${prefix}/Modules/arraymodule.c
    ${prefix}/Modules/atexitmodule.c
    ${prefix}/Modules/binascii.c
    ${prefix}/Modules/cmathmodule.c
    ${prefix}/Modules/_datetimemodule.c
    ${prefix}/Modules/errnomodule.c
    ${prefix}/Modules/faulthandler.c
    ${prefix}/Modules/gcmodule.c
    ${prefix}/Modules/hashtable.c
    ${prefix}/Modules/itertoolsmodule.c
    ${prefix}/Modules/main.c
    ${prefix}/Modules/mathmodule.c
    ${prefix}/Modules/md5module.c
    ${prefix}/Modules/mmapmodule.c
    ${prefix}/Modules/_opcode.c
    ${prefix}/Modules/_operator.c
    ${prefix}/Modules/parsermodule.c
    ${prefix}/Modules/posixmodule.c
    ${prefix}/Modules/rotatingtree.c
    ${prefix}/Modules/sha1module.c
    ${prefix}/Modules/sha256module.c
    ${prefix}/Modules/sha512module.c
    ${prefix}/Modules/signalmodule.c
    ${prefix}/Modules/symtablemodule.c
    ${prefix}/Modules/_threadmodule.c
    ${prefix}/Modules/_tracemalloc.c
    ${prefix}/Modules/timemodule.c
    ${prefix}/Modules/xxsubtype.c
    ${prefix}/Modules/_io/fileio.c
    ${prefix}/Modules/_io/bytesio.c
    ${prefix}/Modules/_io/stringio.c
    ${prefix}/Modules/_io/bufferedio.c
    ${prefix}/Modules/_io/iobase.c
    ${prefix}/Modules/_io/textio.c
    ${prefix}/Modules/_io/_iomodule.c
    ${prefix}/Modules/cjkcodecs/_codecs_cn.c
    ${prefix}/Modules/cjkcodecs/_codecs_hk.c
    ${prefix}/Modules/cjkcodecs/_codecs_iso2022.c
    ${prefix}/Modules/cjkcodecs/_codecs_jp.c
    ${prefix}/Modules/cjkcodecs/_codecs_kr.c
    ${prefix}/Modules/cjkcodecs/_codecs_tw.c
    ${prefix}/Modules/cjkcodecs/multibytecodec.c
    ${prefix}/Objects/abstract.c
    ${prefix}/Objects/accu.c
    ${prefix}/Objects/boolobject.c
    ${prefix}/Objects/bytes_methods.c
    ${prefix}/Objects/bytearrayobject.c
    ${prefix}/Objects/bytesobject.c
    ${prefix}/Objects/capsule.c
    ${prefix}/Objects/cellobject.c
    ${prefix}/Objects/classobject.c
    ${prefix}/Objects/codeobject.c
    ${prefix}/Objects/complexobject.c
    ${prefix}/Objects/descrobject.c
    ${prefix}/Objects/dictobject.c
    ${prefix}/Objects/enumobject.c
    ${prefix}/Objects/exceptions.c
    ${prefix}/Objects/fileobject.c
    ${prefix}/Objects/floatobject.c
    ${prefix}/Objects/frameobject.c
    ${prefix}/Objects/funcobject.c
    ${prefix}/Objects/genobject.c
    ${prefix}/Objects/iterobject.c
    ${prefix}/Objects/listobject.c
    ${prefix}/Objects/longobject.c
    ${prefix}/Objects/memoryobject.c
    ${prefix}/Objects/methodobject.c
    ${prefix}/Objects/moduleobject.c
    ${prefix}/Objects/namespaceobject.c
    ${prefix}/Objects/object.c
    ${prefix}/Objects/obmalloc.c
    ${prefix}/Objects/rangeobject.c
    ${prefix}/Objects/setobject.c
    ${prefix}/Objects/sliceobject.c
    ${prefix}/Objects/structseq.c
    ${prefix}/Objects/tupleobject.c
    ${prefix}/Objects/typeobject.c
    ${prefix}/Objects/unicodectype.c
    ${prefix}/Objects/unicodeobject.c
    ${prefix}/Objects/weakrefobject.c
    ${prefix}/Parser/acceler.c
    ${prefix}/Parser/bitset.c
    ${prefix}/Parser/firstsets.c
    ${prefix}/Parser/grammar.c
    ${prefix}/Parser/grammar1.c
    ${prefix}/Parser/listnode.c
    ${prefix}/Parser/metagrammar.c
    ${prefix}/Parser/myreadline.c
    ${prefix}/Parser/node.c
    ${prefix}/Parser/parser.c
    ${prefix}/Parser/parsetok.c
    ${prefix}/Parser/tokenizer.c
    ${prefix}/Python/pyhash.c
    ${prefix}/Python/random.c
    ${prefix}/Python/_warnings.c
    ${prefix}/Python/asdl.c
    ${prefix}/Python/ast.c
    ${prefix}/Python/bltinmodule.c
    ${prefix}/Python/ceval.c
    ${prefix}/Python/codecs.c
    ${prefix}/Python/compile.c
    ${prefix}/Python/dynamic_annotations.c
    ${prefix}/Python/errors.c
    ${prefix}/Python/fileutils.c
    ${prefix}/Python/formatter_unicode.c
    ${prefix}/Python/frozen.c
    ${prefix}/Python/future.c
    ${prefix}/Python/getargs.c
    ${prefix}/Python/getcompiler.c
    ${prefix}/Python/getcopyright.c
    ${prefix}/Python/getopt.c
    ${prefix}/Python/getplatform.c
    ${prefix}/Python/getversion.c
    ${prefix}/Python/graminit.c
    ${prefix}/Python/import.c
    ${prefix}/Python/importdl.c
    ${prefix}/Python/marshal.c
    ${prefix}/Python/modsupport.c
    ${prefix}/Python/mysnprintf.c
    ${prefix}/Python/mystrtoul.c
    ${prefix}/Python/peephole.c
    ${prefix}/Python/pyarena.c
    ${prefix}/Python/pyctype.c
    ${prefix}/Python/pyfpe.c
    ${prefix}/Python/pymath.c
    ${prefix}/Python/pytime.c
    ${prefix}/Python/pystate.c
    ${prefix}/Python/pystrcmp.c
    ${prefix}/Python/pystrtod.c
    ${prefix}/Python/dtoa.c
    ${prefix}/Python/Python-ast.c
    ${prefix}/Python/pythonrun.c
    ${prefix}/Python/structmember.c
    ${prefix}/Python/symtable.c
    ${prefix}/Python/sysmodule.c
    ${prefix}/Python/thread.c
    ${prefix}/Python/traceback.c )

IF(CMAKE_GENERATOR MATCHES Visual*)
    SET( pythoncore_sources
	${pythoncore_sources}
	${prefix}/platform/Windows/winreg.c
    ${prefix}/platform/Windows/config.c
    ${prefix}/platform/Windows/dl_nt.c
    ${prefix}/platform/Windows/getpathp.c
    ${prefix}/platform/Windows/msvcrtmodule.c
    ${prefix}/Modules/_winapi.c
    ${prefix}/Python/dynload_win.c)
ELSE()
    SET( pythoncore_sources
    ${pythoncore_sources}
    ${prefix}/platform/Linux/config.c
    ${prefix}/Modules/getpath.c
    ${prefix}/Modules/pwdmodule.c
    ${prefix}/Modules/_posixsubprocess.c
    ${prefix}/Modules/selectmodule.c
    ${prefix}/Python/dynload_shlib.c )
ENDIF()

IF(CMAKE_GENERATOR MATCHES Visual*)
    INCLUDE_DIRECTORIES( ${prefix}/platform/Windows )
ELSE()
    INCLUDE_DIRECTORIES( ${prefix}/platform/Linux )
ENDIF()

INCLUDE_DIRECTORIES( ${prefix}/Include )

ADD_DEFINITIONS( -D_USRDLL -DPy_BUILD_CORE -DPy_ENABLE_SHARED -DSOABI=\"cpython-34m\" -DPREFIX=\".\" -DEXEC_PREFIX=\".\" -DVERSION=\"3.4\" -DVPATH=\"\")

IF(CMAKE_GENERATOR MATCHES Visual*)
    ADD_DEFINITIONS( -DWIN32 )
ENDIF()

ADD_LIBRARY( python34 SHARED ${pythoncore_sources} ${pythoncore_headers} )

INSTALL( TARGETS python34 DESTINATION . )

IF( IS_64BIT )
    SET_PROPERTY( TARGET python34 APPEND PROPERTY COMPILE_FLAGS "-fPIC")
ENDIF()

SET( prefix ${ROOT_PATH}/src/plugins/python34 )

INSTALL( DIRECTORY ${prefix}/Lib/encodings DESTINATION ./pythonlibs )
INSTALL( FILES
		${prefix}/Lib/_bootlocale.py
		${prefix}/Lib/_collections_abc.py
		${prefix}/Lib/_sitebuiltins.py
		${prefix}/Lib/_weakrefset.py
		${prefix}/Lib/abc.py
		${prefix}/Lib/codecs.py
		${prefix}/Lib/genericpath.py
		${prefix}/Lib/io.py
		${prefix}/Lib/ntpath.py
		${prefix}/Lib/os.py
		${prefix}/Lib/posixpath.py
		${prefix}/Lib/site.py
		${prefix}/Lib/stat.py
		${prefix}/Lib/sysconfig.py
		DESTINATION ./pythonlibs )
