# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

FREETYPE_SRC_PATH	:= freetype2-android/
LUA_SRC_PATH 		:= lua-5.1.4/
PHYSFS_SRC_PATH 	:= physfs-2.0.2/
LOVE_SRC_PATH 		:= love/
TIFF_SRC_PATH 		:= tiff-3.9.5/
MNG_SRC_PATH 		:= libmng-1.0.10/
LCMS_SRC_PATH 		:= lcms2-2.2/
PNG_SRC_PATH 		:= libpng-1.5.6/
JPEG_SRC_PATH 		:= jpeg-8c/
JASPER_SRC_PATH 	:= jasper-1.900.1/
DEVIL_SRC_PATH 		:= devil-1.7.8/
OPENAL_SRC_PATH		:= openal-soft-android/
MPG123_SRC_PATH		:= mpg123-1.13.4/
VORBIS_SRC_PATH		:= libvorbis-1.3.2/
OGG_SRC_PATH		:= libogg-1.3.0/
MODPLUG_SRC_PATH	:= libmodplug-0.8.8.4/

# libogg
include $(CLEAR_VARS)

LOCAL_MODULE    := libogg
LOCAL_CFLAGS    := -fexceptions -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${OGG_SRC_PATH}/include
		
LOCAL_SRC_FILES := \
	${OGG_SRC_PATH}src/framing.c \
	${OGG_SRC_PATH}src/bitwise.c \


LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

include $(BUILD_SHARED_LIBRARY)

#libvorbis
include $(CLEAR_VARS)

LOCAL_MODULE    := libvorbis
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${VORBIS_SRC_PATH}include \
	${OGG_SRC_PATH}/include
		
LOCAL_SRC_FILES := \
	${VORBIS_SRC_PATH}lib/mdct.c \
	${VORBIS_SRC_PATH}lib/smallft.c \
	${VORBIS_SRC_PATH}lib/block.c \
	${VORBIS_SRC_PATH}lib/envelope.c \
	${VORBIS_SRC_PATH}lib/window.c \
	${VORBIS_SRC_PATH}lib/lsp.c \
	${VORBIS_SRC_PATH}lib/lpc.c \
	${VORBIS_SRC_PATH}lib/analysis.c \
	${VORBIS_SRC_PATH}lib/synthesis.c \
	${VORBIS_SRC_PATH}lib/psy.c \
	${VORBIS_SRC_PATH}lib/info.c \
	${VORBIS_SRC_PATH}lib/floor1.c \
	${VORBIS_SRC_PATH}lib/floor0.c \
	${VORBIS_SRC_PATH}lib/res0.c \
	${VORBIS_SRC_PATH}lib/mapping0.c \
	${VORBIS_SRC_PATH}lib/registry.c \
	${VORBIS_SRC_PATH}lib/codebook.c \
	${VORBIS_SRC_PATH}lib/sharedbook.c \
	${VORBIS_SRC_PATH}lib/lookup.c \
	${VORBIS_SRC_PATH}lib/bitrate.c \
	${VORBIS_SRC_PATH}lib/vorbisfile.c 


LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := libogg

include $(BUILD_SHARED_LIBRARY)

#libmpg123
include $(CLEAR_VARS)

LOCAL_MODULE    := libmpg123
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  := ${LUA_SRC_PATH}src \
	${OPENAL_SRC_PATH}include/ \
	${OPENAL_SRC_PATH}OpenAL32/Include \
	${TIFF_SRC_PATH}libtiff/ \
	${MNG_SRC_PATH} \
	${LCMS_SRC_PATH}include/ \
	${PNG_SRC_PATH} \
	${JPEG_SRC_PATH} \
	${JASPER_SRC_PATH}src/libjasper/include \
	${DEVIL_SRC_PATH}include \
	${DEVIL_SRC_PATH}src-IL/include \
	${LOVE_SRC_PATH}src \
	${LOVE_SRC_PATH}src/modules \
	$(FREETYPE_SRC_PATH)include \
	$(FREETYPE_SRC_PATH)src \
	${PHYSFS_SRC_PATH} \
	${MPG123_SRC_PATH}src/libmpg123/ \
	${MODPLUG_SRC_PATH}src/ \
	${MODPLUG_SRC_PATH}src/libmodplug \
	${VORBIS_SRC_PATH}include \
	${OGG_SRC_PATH}/include
		
LOCAL_SRC_FILES := \
	${MPG123_SRC_PATH}src/libmpg123/compat.c \
	${MPG123_SRC_PATH}src/libmpg123/parse.c \
	${MPG123_SRC_PATH}src/libmpg123/frame.c \
	${MPG123_SRC_PATH}src/libmpg123/format.c \
	${MPG123_SRC_PATH}src/libmpg123/dct64.c \
	${MPG123_SRC_PATH}src/libmpg123/equalizer.c \
	${MPG123_SRC_PATH}src/libmpg123/id3.c \
	${MPG123_SRC_PATH}src/libmpg123/optimize.c \
	${MPG123_SRC_PATH}src/libmpg123/readers.c \
	${MPG123_SRC_PATH}src/libmpg123/tabinit.c \
	${MPG123_SRC_PATH}src/libmpg123/libmpg123.c \
	${MPG123_SRC_PATH}src/libmpg123/index.c \
	${MPG123_SRC_PATH}src/libmpg123/stringbuf.c \
	${MPG123_SRC_PATH}src/libmpg123/icy.c \
	${MPG123_SRC_PATH}src/libmpg123/icy2utf8.c \
	${MPG123_SRC_PATH}src/libmpg123/ntom.c \
	${MPG123_SRC_PATH}src/libmpg123/synth.c \
	${MPG123_SRC_PATH}src/libmpg123/synth_8bit.c \
	${MPG123_SRC_PATH}src/libmpg123/layer1.c \
	${MPG123_SRC_PATH}src/libmpg123/layer2.c \
	${MPG123_SRC_PATH}src/libmpg123/layer3.c \
	${MPG123_SRC_PATH}src/libmpg123/synth_s32.c \
	${MPG123_SRC_PATH}src/libmpg123/synth_real.c \
	${MPG123_SRC_PATH}src/libmpg123/feature.c \


LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

 LOCAL_STATIC_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)

#libmodplug
include $(CLEAR_VARS)

LOCAL_MODULE    := libmodplug
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${MODPLUG_SRC_PATH}src/ \
	${MODPLUG_SRC_PATH}src/libmodplug
		
LOCAL_SRC_FILES := \
	${MODPLUG_SRC_PATH}src/sndmix.cpp \
	${MODPLUG_SRC_PATH}src/sndfile.cpp \
	${MODPLUG_SRC_PATH}src/snd_fx.cpp \
	${MODPLUG_SRC_PATH}src/snd_flt.cpp \
	${MODPLUG_SRC_PATH}src/snd_dsp.cpp \
	${MODPLUG_SRC_PATH}src/fastmix.cpp \
	${MODPLUG_SRC_PATH}src/mmcmp.cpp \
	${MODPLUG_SRC_PATH}src/load_xm.cpp \
	${MODPLUG_SRC_PATH}src/load_wav.cpp \
	${MODPLUG_SRC_PATH}src/load_umx.cpp \
	${MODPLUG_SRC_PATH}src/load_ult.cpp \
	${MODPLUG_SRC_PATH}src/load_stm.cpp \
	${MODPLUG_SRC_PATH}src/load_s3m.cpp \
	${MODPLUG_SRC_PATH}src/load_ptm.cpp \
	${MODPLUG_SRC_PATH}src/load_okt.cpp \
	${MODPLUG_SRC_PATH}src/load_mtm.cpp \
	${MODPLUG_SRC_PATH}src/load_mod.cpp \
	${MODPLUG_SRC_PATH}src/load_med.cpp \
	${MODPLUG_SRC_PATH}src/load_mdl.cpp \
	${MODPLUG_SRC_PATH}src/load_it.cpp \
	${MODPLUG_SRC_PATH}src/load_far.cpp \
	${MODPLUG_SRC_PATH}src/load_dsm.cpp \
	${MODPLUG_SRC_PATH}src/load_dmf.cpp \
	${MODPLUG_SRC_PATH}src/load_dbm.cpp \
	${MODPLUG_SRC_PATH}src/load_ams.cpp \
	${MODPLUG_SRC_PATH}src/load_amf.cpp \
	${MODPLUG_SRC_PATH}src/load_669.cpp \
	${MODPLUG_SRC_PATH}src/load_j2b.cpp \
	${MODPLUG_SRC_PATH}src/load_mt2.cpp \
	${MODPLUG_SRC_PATH}src/load_psm.cpp \
	${MODPLUG_SRC_PATH}src/load_abc.cpp \
	${MODPLUG_SRC_PATH}src/load_mid.cpp \
	${MODPLUG_SRC_PATH}src/load_pat.cpp \
	${MODPLUG_SRC_PATH}src/modplug.cpp \


LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)

#libopenal
include $(CLEAR_VARS)

LOCAL_MODULE    := libopenal
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${OPENAL_SRC_PATH}include/ \
	${OPENAL_SRC_PATH}OpenAL32/Include \
	${OPENAL_SRC_PATH}
		
LOCAL_SRC_FILES := \
	${OPENAL_SRC_PATH}OpenAL32/alAuxEffectSlot.c \
	${OPENAL_SRC_PATH}OpenAL32/alBuffer.c \
	${OPENAL_SRC_PATH}OpenAL32/alEffect.c \
	${OPENAL_SRC_PATH}OpenAL32/alError.c \
	${OPENAL_SRC_PATH}OpenAL32/alExtension.c \
	${OPENAL_SRC_PATH}OpenAL32/alFilter.c \
	${OPENAL_SRC_PATH}OpenAL32/alListener.c \
	${OPENAL_SRC_PATH}OpenAL32/alSource.c \
	${OPENAL_SRC_PATH}OpenAL32/alState.c \
	${OPENAL_SRC_PATH}OpenAL32/alThunk.c \
	${OPENAL_SRC_PATH}Alc/ALc.c \
	${OPENAL_SRC_PATH}Alc/ALu.c \
	${OPENAL_SRC_PATH}Alc/alcConfig.c \
	${OPENAL_SRC_PATH}Alc/alcDedicated.c \
	${OPENAL_SRC_PATH}Alc/alcEcho.c \
	${OPENAL_SRC_PATH}Alc/alcModulator.c \
	${OPENAL_SRC_PATH}Alc/alcReverb.c \
	${OPENAL_SRC_PATH}Alc/alcRing.c \
	${OPENAL_SRC_PATH}Alc/alcThread.c \
	${OPENAL_SRC_PATH}Alc/bs2b.c \
	${OPENAL_SRC_PATH}Alc/helpers.c \
	${OPENAL_SRC_PATH}Alc/hrtf.c \
	${OPENAL_SRC_PATH}Alc/mixer.c \
	${OPENAL_SRC_PATH}Alc/panning.c \
	${OPENAL_SRC_PATH}Alc/backends/loopback.c \
	${OPENAL_SRC_PATH}Alc/backends/null.c \
	${OPENAL_SRC_PATH}Alc/backends/android.c \
	${OPENAL_SRC_PATH}Alc/backends/wave.c 

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)

#libjpeg
include $(CLEAR_VARS)

LOCAL_MODULE    := libjpeg
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${JPEG_SRC_PATH} 
		
LOCAL_SRC_FILES := \
	${JPEG_SRC_PATH}jquant1.c \
	${JPEG_SRC_PATH}jdtrans.c \
	${JPEG_SRC_PATH}jerror.c \
	${JPEG_SRC_PATH}wrppm.c \
	${JPEG_SRC_PATH}jcparam.c \
	${JPEG_SRC_PATH}jdmarker.c \
	${JPEG_SRC_PATH}jidctfst.c \
	${JPEG_SRC_PATH}jctrans.c \
	${JPEG_SRC_PATH}cdjpeg.c \
	${JPEG_SRC_PATH}jcomapi.c \
	${JPEG_SRC_PATH}jmemansi.c \
	${JPEG_SRC_PATH}jquant2.c \
	${JPEG_SRC_PATH}wrtarga.c \
	${JPEG_SRC_PATH}transupp.c \
	${JPEG_SRC_PATH}jdmerge.c \
	${JPEG_SRC_PATH}jdhuff.c \
	${JPEG_SRC_PATH}jdmaster.c \
	${JPEG_SRC_PATH}jcprepct.c \
	${JPEG_SRC_PATH}jutils.c \
	${JPEG_SRC_PATH}jccoefct.c \
	${JPEG_SRC_PATH}jccolor.c \
	${JPEG_SRC_PATH}jdatasrc.c \
	${JPEG_SRC_PATH}jcdctmgr.c \
	${JPEG_SRC_PATH}wrrle.c \
	${JPEG_SRC_PATH}jcapimin.c \
	${JPEG_SRC_PATH}jfdctfst.c \
	${JPEG_SRC_PATH}rdrle.c \
	${JPEG_SRC_PATH}wrbmp.c \
	${JPEG_SRC_PATH}jdarith.c \
	${JPEG_SRC_PATH}jidctint.c \
	${JPEG_SRC_PATH}jcsample.c \
	${JPEG_SRC_PATH}jaricom.c \
	${JPEG_SRC_PATH}jdcolor.c \
	${JPEG_SRC_PATH}jdapistd.c \
	${JPEG_SRC_PATH}jdmainct.c \
	${JPEG_SRC_PATH}jddctmgr.c \
	${JPEG_SRC_PATH}jcmaster.c \
	${JPEG_SRC_PATH}jfdctflt.c \
	${JPEG_SRC_PATH}rdgif.c \
	${JPEG_SRC_PATH}jcarith.c \
	${JPEG_SRC_PATH}rdcolmap.c \
	${JPEG_SRC_PATH}wrgif.c \
	${JPEG_SRC_PATH}jdcoefct.c \
	${JPEG_SRC_PATH}rdbmp.c \
	${JPEG_SRC_PATH}jdapimin.c \
	${JPEG_SRC_PATH}jdsample.c \
	${JPEG_SRC_PATH}jidctflt.c \
	${JPEG_SRC_PATH}rdppm.c \
	${JPEG_SRC_PATH}rdtarga.c \
	${JPEG_SRC_PATH}jchuff.c \
	${JPEG_SRC_PATH}jcmarker.c \
	${JPEG_SRC_PATH}jdpostct.c \
	${JPEG_SRC_PATH}jfdctint.c \
	${JPEG_SRC_PATH}jcinit.c \
	${JPEG_SRC_PATH}jmemmgr.c \
	${JPEG_SRC_PATH}jcmainct.c \
	${JPEG_SRC_PATH}jcapistd.c \
	${JPEG_SRC_PATH}rdswitch.c \
	${JPEG_SRC_PATH}jdatadst.c \
	${JPEG_SRC_PATH}jdinput.c 

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)

#libtiff
include $(CLEAR_VARS)

LOCAL_MODULE    := libtiff
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${TIFF_SRC_PATH}libtiff/ \
	${JPEG_SRC_PATH} \
		
LOCAL_SRC_FILES := \
	${TIFF_SRC_PATH}libtiff/tif_dir.c \
	${TIFF_SRC_PATH}libtiff/tif_codec.c \
	${TIFF_SRC_PATH}libtiff/tif_predict.c \
	${TIFF_SRC_PATH}libtiff/tif_tile.c \
	${TIFF_SRC_PATH}libtiff/tif_version.c \
	${TIFF_SRC_PATH}libtiff/tif_unix.c \
	${TIFF_SRC_PATH}libtiff/tif_swab.c \
	${TIFF_SRC_PATH}libtiff/tif_thunder.c \
	${TIFF_SRC_PATH}libtiff/tif_next.c \
	${TIFF_SRC_PATH}libtiff/tif_strip.c \
	${TIFF_SRC_PATH}libtiff/tif_extension.c \
	${TIFF_SRC_PATH}libtiff/tif_error.c \
	${TIFF_SRC_PATH}libtiff/tif_dirwrite.c \
	${TIFF_SRC_PATH}libtiff/tif_fax3sm.c \
	${TIFF_SRC_PATH}libtiff/tif_ojpeg.c \
	${TIFF_SRC_PATH}libtiff/tif_flush.c \
	${TIFF_SRC_PATH}libtiff/tif_warning.c \
	${TIFF_SRC_PATH}libtiff/tif_fax3.c \
	${TIFF_SRC_PATH}libtiff/tif_jbig.c \
	${TIFF_SRC_PATH}libtiff/tif_open.c \
	${TIFF_SRC_PATH}libtiff/tif_write.c \
	${TIFF_SRC_PATH}libtiff/tif_packbits.c \
	${TIFF_SRC_PATH}libtiff/tif_compress.c \
	${TIFF_SRC_PATH}libtiff/tif_color.c \
	${TIFF_SRC_PATH}libtiff/tif_print.c \
	${TIFF_SRC_PATH}libtiff/tif_zip.c \
	${TIFF_SRC_PATH}libtiff/tif_aux.c \
	${TIFF_SRC_PATH}libtiff/tif_dumpmode.c \
	${TIFF_SRC_PATH}libtiff/tif_dirread.c \
	${TIFF_SRC_PATH}libtiff/tif_getimage.c \
	${TIFF_SRC_PATH}libtiff/tif_jpeg.c \
	${TIFF_SRC_PATH}libtiff/tif_close.c \
	${TIFF_SRC_PATH}libtiff/tif_read.c \
	${TIFF_SRC_PATH}libtiff/tif_luv.c \
	${TIFF_SRC_PATH}libtiff/tif_dirinfo.c \
	${TIFF_SRC_PATH}libtiff/tif_lzw.c \
	${TIFF_SRC_PATH}libtiff/tif_pixarlog.c \
	${TIFF_SRC_PATH}port/lfind.c 

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := libjpeg libphysfs

include $(BUILD_SHARED_LIBRARY)

#libphysfs
include $(CLEAR_VARS)

LOCAL_MODULE    := libphysfs
LOCAL_CFLAGS    := -g -Dlinux -Dunix -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT \
					-DPHYSFS_SUPPORTS_ZIP=1 -DPHYSFS_SUPPORTS_ZIP=1 -DPHYSFS_SUPPORTS_7Z=1 -DPHYSFS_SUPPORTS_GRP=1 -DPHYSFS_SUPPORTS_QPAK=1 -DPHYSFS_SUPPORTS_HOG=1 -DPHYSFS_SUPPORTS_MVL=1 -DPHYSFS_SUPPORTS_WAD=1

LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${PHYSFS_SRC_PATH} 
		
LOCAL_SRC_FILES := \
	${PHYSFS_SRC_PATH}/physfs.c \
	${PHYSFS_SRC_PATH}/physfs_byteorder.c \
	${PHYSFS_SRC_PATH}/physfs_unicode.c \
	${PHYSFS_SRC_PATH}/platform/os2.c \
	${PHYSFS_SRC_PATH}/platform/pocketpc.c \
	${PHYSFS_SRC_PATH}/platform/posix.c \
	${PHYSFS_SRC_PATH}/platform/unix.c \
	${PHYSFS_SRC_PATH}/platform/macosx.c \
	${PHYSFS_SRC_PATH}/platform/windows.c \
	${PHYSFS_SRC_PATH}/archivers/dir.c \
	${PHYSFS_SRC_PATH}/archivers/grp.c \
	${PHYSFS_SRC_PATH}/archivers/hog.c \
	${PHYSFS_SRC_PATH}/archivers/lzma.c \
	${PHYSFS_SRC_PATH}/archivers/mvl.c \
	${PHYSFS_SRC_PATH}/archivers/qpak.c \
	${PHYSFS_SRC_PATH}/archivers/wad.c \
	${PHYSFS_SRC_PATH}/archivers/zip.c \
	${PHYSFS_SRC_PATH}/lzma/C/7zCrc.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zBuffer.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zDecode.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zExtract.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zHeader.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zIn.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zItem.c \
	${PHYSFS_SRC_PATH}/lzma/C/Archive/7z/7zMethodID.c \
	${PHYSFS_SRC_PATH}/lzma/C/Compress/Branch/BranchX86.c \
	${PHYSFS_SRC_PATH}/lzma/C/Compress/Branch/BranchX86_2.c \
	${PHYSFS_SRC_PATH}/lzma/C/Compress/Lzma/LzmaDecode.c \
	${PHYSFS_SRC_PATH}/zlib123/adler32.c \
	${PHYSFS_SRC_PATH}/zlib123/compress.c \
	${PHYSFS_SRC_PATH}/zlib123/crc32.c \
	${PHYSFS_SRC_PATH}/zlib123/deflate.c \
	${PHYSFS_SRC_PATH}/zlib123/gzio.c \
	${PHYSFS_SRC_PATH}/zlib123/infback.c \
	${PHYSFS_SRC_PATH}/zlib123/inffast.c \
	${PHYSFS_SRC_PATH}/zlib123/inflate.c \
	${PHYSFS_SRC_PATH}/zlib123/inftrees.c \
	${PHYSFS_SRC_PATH}/zlib123/trees.c \
	${PHYSFS_SRC_PATH}/zlib123/uncompr.c \
	${PHYSFS_SRC_PATH}/zlib123/zutil.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)

#libmng
include $(CLEAR_VARS)

LOCAL_MODULE    := libmng
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${JPEG_SRC_PATH} \
	${MNG_SRC_PATH}
		
LOCAL_SRC_FILES := \
	${MNG_SRC_PATH}libmng_write.c \
	${MNG_SRC_PATH}libmng_object_prc.c \
	${MNG_SRC_PATH}libmng_display.c \
	${MNG_SRC_PATH}libmng_chunk_xs.c \
	${MNG_SRC_PATH}libmng_chunk_io.c \
	${MNG_SRC_PATH}libmng_read.c \
	${MNG_SRC_PATH}libmng_prop_xs.c \
	${MNG_SRC_PATH}libmng_filter.c \
	${MNG_SRC_PATH}libmng_chunk_descr.c \
	${MNG_SRC_PATH}libmng_cms.c \
	${MNG_SRC_PATH}libmng_zlib.c \
	${MNG_SRC_PATH}libmng_pixels.c \
	${MNG_SRC_PATH}libmng_error.c \
	${MNG_SRC_PATH}libmng_trace.c \
	${MNG_SRC_PATH}libmng_jpeg.c \
	${MNG_SRC_PATH}libmng_callback_xs.c \
	${MNG_SRC_PATH}libmng_dither.c \
	${MNG_SRC_PATH}libmng_hlapi.c \
	${MNG_SRC_PATH}libmng_chunk_prc.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := libjpeg libphysfs

include $(BUILD_SHARED_LIBRARY)

#liblua
include $(CLEAR_VARS)

LOCAL_MODULE    := liblua
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${LUA_SRC_PATH}src 
		
LOCAL_SRC_FILES := \
	${LUA_SRC_PATH}src/loadlib.c \
	${LUA_SRC_PATH}src/lvm.c \
	${LUA_SRC_PATH}src/ldo.c \
	${LUA_SRC_PATH}src/lstate.c \
	${LUA_SRC_PATH}src/ltablib.c \
	${LUA_SRC_PATH}src/ltm.c \
	${LUA_SRC_PATH}src/linit.c \
	${LUA_SRC_PATH}src/liolib.c \
	${LUA_SRC_PATH}src/ldblib.c \
	${LUA_SRC_PATH}src/lcode.c \
	${LUA_SRC_PATH}src/loslib.c \
	${LUA_SRC_PATH}src/lgc.c \
	${LUA_SRC_PATH}src/print.c \
	${LUA_SRC_PATH}src/llex.c \
	${LUA_SRC_PATH}src/lfunc.c \
	${LUA_SRC_PATH}src/lparser.c \
	${LUA_SRC_PATH}src/ldump.c \
	${LUA_SRC_PATH}src/lundump.c \
	${LUA_SRC_PATH}src/lmathlib.c \
	${LUA_SRC_PATH}src/lauxlib.c \
	${LUA_SRC_PATH}src/lopcodes.c \
	${LUA_SRC_PATH}src/lapi.c \
	${LUA_SRC_PATH}src/lmem.c \
	${LUA_SRC_PATH}src/lstrlib.c \
	${LUA_SRC_PATH}src/lzio.c \
	${LUA_SRC_PATH}src/lstring.c \
	${LUA_SRC_PATH}src/lbaselib.c \
	${LUA_SRC_PATH}src/ltable.c \
	${LUA_SRC_PATH}src/ldebug.c \
	${LUA_SRC_PATH}src/lobject.c 

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES :=  

include $(BUILD_SHARED_LIBRARY)

#libfreetype
include $(CLEAR_VARS)

LOCAL_MODULE    := libfreetype
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	$(FREETYPE_SRC_PATH)include \
	$(FREETYPE_SRC_PATH)src \
		
LOCAL_SRC_FILES := \
    $(FREETYPE_SRC_PATH)src/autofit/autofit.c \
    $(FREETYPE_SRC_PATH)src/base/basepic.c \
    $(FREETYPE_SRC_PATH)src/base/ftapi.c \
    $(FREETYPE_SRC_PATH)src/base/ftbase.c \
    $(FREETYPE_SRC_PATH)src/base/ftbbox.c \
    $(FREETYPE_SRC_PATH)src/base/ftbitmap.c \
    $(FREETYPE_SRC_PATH)src/base/ftdbgmem.c \
    $(FREETYPE_SRC_PATH)src/base/ftdebug.c \
    $(FREETYPE_SRC_PATH)src/base/ftglyph.c \
    $(FREETYPE_SRC_PATH)src/base/ftinit.c \
    $(FREETYPE_SRC_PATH)src/base/ftpic.c \
    $(FREETYPE_SRC_PATH)src/base/ftstroke.c \
    $(FREETYPE_SRC_PATH)src/base/ftsynth.c \
    $(FREETYPE_SRC_PATH)src/base/ftsystem.c \
    $(FREETYPE_SRC_PATH)src/cff/cff.c \
    $(FREETYPE_SRC_PATH)src/pshinter/pshinter.c \
    $(FREETYPE_SRC_PATH)src/psnames/psnames.c \
    $(FREETYPE_SRC_PATH)src/raster/raster.c \
    $(FREETYPE_SRC_PATH)src/sfnt/sfnt.c \
    $(FREETYPE_SRC_PATH)src/smooth/smooth.c \
    $(FREETYPE_SRC_PATH)src/truetype/truetype.c 

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES :=  

include $(BUILD_SHARED_LIBRARY)

#liblcms
include $(CLEAR_VARS)

LOCAL_MODULE    := liblcms
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${LCMS_SRC_PATH}include/ \
		
LOCAL_SRC_FILES := \
	${LCMS_SRC_PATH}src/cmscam02.c \
	${LCMS_SRC_PATH}src/cmserr.c \
	${LCMS_SRC_PATH}src/cmstypes.c \
	${LCMS_SRC_PATH}src/cmsmd5.c \
	${LCMS_SRC_PATH}src/cmswtpnt.c \
	${LCMS_SRC_PATH}src/cmsgmt.c \
	${LCMS_SRC_PATH}src/cmspcs.c \
	${LCMS_SRC_PATH}src/cmsopt.c \
	${LCMS_SRC_PATH}src/cmscnvrt.c \
	${LCMS_SRC_PATH}src/cmsps2.c \
	${LCMS_SRC_PATH}src/cmssamp.c \
	${LCMS_SRC_PATH}src/cmsplugin.c \
	${LCMS_SRC_PATH}src/cmsmtrx.c \
	${LCMS_SRC_PATH}src/cmspack.c \
	${LCMS_SRC_PATH}src/cmsio0.c \
	${LCMS_SRC_PATH}src/cmsgamma.c \
	${LCMS_SRC_PATH}src/cmscgats.c \
	${LCMS_SRC_PATH}src/cmsxform.c \
	${LCMS_SRC_PATH}src/cmsnamed.c \
	${LCMS_SRC_PATH}src/cmssm.c \
	${LCMS_SRC_PATH}src/cmsio1.c \
	${LCMS_SRC_PATH}src/cmsintrp.c \
	${LCMS_SRC_PATH}src/cmslut.c \
	${LCMS_SRC_PATH}src/cmsvirt.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES :=  

include $(BUILD_SHARED_LIBRARY)

#libpng
include $(CLEAR_VARS)

LOCAL_MODULE    := libpng
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${PNG_SRC_PATH} \
		
LOCAL_SRC_FILES := \
	${PNG_SRC_PATH}pngerror.c \
	${PNG_SRC_PATH}pngwio.c \
	${PNG_SRC_PATH}pngwrite.c \
	${PNG_SRC_PATH}pngwutil.c \
	${PNG_SRC_PATH}pngwtran.c \
	${PNG_SRC_PATH}pngset.c \
	${PNG_SRC_PATH}pngtrans.c \
	${PNG_SRC_PATH}example.c \
	${PNG_SRC_PATH}png.c \
	${PNG_SRC_PATH}pngrtran.c \
	${PNG_SRC_PATH}pngmem.c \
	${PNG_SRC_PATH}pngpread.c \
	${PNG_SRC_PATH}pngrutil.c \
	${PNG_SRC_PATH}pngrio.c \
	${PNG_SRC_PATH}pngget.c \
	${PNG_SRC_PATH}pngread.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := libphysfs

include $(BUILD_SHARED_LIBRARY)

#libjasper
include $(CLEAR_VARS)

LOCAL_MODULE    := libjasper
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${JASPER_SRC_PATH}src/libjasper/include \
	${JPEG_SRC_PATH} \
		
LOCAL_SRC_FILES := \
	${JASPER_SRC_PATH}src/libjasper/base/jas_cm.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_getopt.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_seq.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_malloc.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_init.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_debug.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_stream.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_string.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_version.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_tmr.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_image.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_tvp.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_iccdata.c \
	${JASPER_SRC_PATH}src/libjasper/base/jas_icc.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_mqcod.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_t1enc.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_dec.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_math.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_util.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_bs.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_tagtree.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_t1cod.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_t2enc.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_mqenc.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_enc.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_qmfb.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_t1dec.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_cs.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_t2cod.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_tsfb.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_mqdec.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_t2dec.c \
	${JASPER_SRC_PATH}src/libjasper/jpc/jpc_mct.c \
	${JASPER_SRC_PATH}src/libjasper/dummy.c \
	${JASPER_SRC_PATH}src/libjasper/pnm/pnm_dec.c \
	${JASPER_SRC_PATH}src/libjasper/pnm/pnm_enc.c \
	${JASPER_SRC_PATH}src/libjasper/pnm/pnm_cod.c \
	${JASPER_SRC_PATH}src/libjasper/jp2/jp2_enc.c \
	${JASPER_SRC_PATH}src/libjasper/jp2/jp2_cod.c \
	${JASPER_SRC_PATH}src/libjasper/jp2/jp2_dec.c \
	${JASPER_SRC_PATH}src/libjasper/pgx/pgx_dec.c \
	${JASPER_SRC_PATH}src/libjasper/pgx/pgx_enc.c \
	${JASPER_SRC_PATH}src/libjasper/pgx/pgx_cod.c \
	${JASPER_SRC_PATH}src/libjasper/bmp/bmp_enc.c \
	${JASPER_SRC_PATH}src/libjasper/bmp/bmp_dec.c \
	${JASPER_SRC_PATH}src/libjasper/bmp/bmp_cod.c \
	${JASPER_SRC_PATH}src/libjasper/jpg/jpg_val.c \
	${JASPER_SRC_PATH}src/libjasper/jpg/jpg_enc.c \
	${JASPER_SRC_PATH}src/libjasper/jpg/jpg_dec.c \
	${JASPER_SRC_PATH}src/libjasper/ras/ras_cod.c \
	${JASPER_SRC_PATH}src/libjasper/ras/ras_dec.c \
	${JASPER_SRC_PATH}src/libjasper/ras/ras_enc.c \
	${JASPER_SRC_PATH}src/libjasper/mif/mif_cod.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := libjpeg

include $(BUILD_SHARED_LIBRARY)

#libdevil
include $(CLEAR_VARS)

LOCAL_MODULE    := libdevil
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT -fexceptions
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  :=  \
	${DEVIL_SRC_PATH}include \
	${DEVIL_SRC_PATH}src-IL/include \
	${JASPER_SRC_PATH}src/libjasper/include \
	${PNG_SRC_PATH} \
	${MNG_SRC_PATH} \
	${JPEG_SRC_PATH} \
	${LCMS_SRC_PATH}include/ \
	${TIFF_SRC_PATH}libtiff/ \
		
LOCAL_SRC_FILES := \
	${DEVIL_SRC_PATH}src-IL/src/il_sun.c \
	${DEVIL_SRC_PATH}src-IL/src/il_wdp.c \
	${DEVIL_SRC_PATH}src-IL/src/il_doom.c \
	${DEVIL_SRC_PATH}src-IL/src/il_jp2.c \
	${DEVIL_SRC_PATH}src-IL/src/il_tpl.c \
	${DEVIL_SRC_PATH}src-IL/src/il_size.c \
	${DEVIL_SRC_PATH}src-IL/src/il_rle.c \
	${DEVIL_SRC_PATH}src-IL/src/il_gif.c \
	${DEVIL_SRC_PATH}src-IL/src/il_states.c \
	${DEVIL_SRC_PATH}src-IL/src/il_psp.c \
	${DEVIL_SRC_PATH}src-IL/src/il_devil.c \
	${DEVIL_SRC_PATH}src-IL/src/il_convbuff.c \
	${DEVIL_SRC_PATH}src-IL/src/il_ftx.c \
	${DEVIL_SRC_PATH}src-IL/src/il_vtf.c \
	${DEVIL_SRC_PATH}src-IL/src/il_iff.c \
	${DEVIL_SRC_PATH}src-IL/src/il_lif.c \
	${DEVIL_SRC_PATH}src-IL/src/il_pcx.c \
	${DEVIL_SRC_PATH}src-IL/src/il_io.c \
	${DEVIL_SRC_PATH}src-IL/src/il_files.c \
	${DEVIL_SRC_PATH}src-IL/src/il_fastconv.c \
	${DEVIL_SRC_PATH}src-IL/src/il_error.c \
	${DEVIL_SRC_PATH}src-IL/src/il_main.c \
	${DEVIL_SRC_PATH}src-IL/src/il_utx.cpp \
	${DEVIL_SRC_PATH}src-IL/src/il_pcd.c \
	${DEVIL_SRC_PATH}src-IL/src/il_neuquant.c \
	${DEVIL_SRC_PATH}src-IL/src/il_dds.c \
	${DEVIL_SRC_PATH}src-IL/src/il_dicom.c \
	${DEVIL_SRC_PATH}src-IL/src/il_wal.c \
	${DEVIL_SRC_PATH}src-IL/src/il_icon.c \
	${DEVIL_SRC_PATH}src-IL/src/il_blp.c \
	${DEVIL_SRC_PATH}src-IL/src/il_mp3.c \
	${DEVIL_SRC_PATH}src-IL/src/il_header.c \
	${DEVIL_SRC_PATH}src-IL/src/il_alloc.c \
	${DEVIL_SRC_PATH}src-IL/src/il_sgi.c \
	${DEVIL_SRC_PATH}src-IL/src/il_stack.c \
	${DEVIL_SRC_PATH}src-IL/src/il_texture.c \
	${DEVIL_SRC_PATH}src-IL/src/altivec_typeconversion.c \
	${DEVIL_SRC_PATH}src-IL/src/il_targa.c \
	${DEVIL_SRC_PATH}src-IL/src/il_bmp.c \
	${DEVIL_SRC_PATH}src-IL/src/il_mdl.c \
	${DEVIL_SRC_PATH}src-IL/src/il_pix.c \
	${DEVIL_SRC_PATH}src-IL/src/il_pnm.c \
	${DEVIL_SRC_PATH}src-IL/src/il_ilbm.c \
	${DEVIL_SRC_PATH}src-IL/src/il_icns.c \
	${DEVIL_SRC_PATH}src-IL/src/il_cut.c \
	${DEVIL_SRC_PATH}src-IL/src/il_profiles.c \
	${DEVIL_SRC_PATH}src-IL/src/il_hdr.c \
	${DEVIL_SRC_PATH}src-IL/src/il_register.c \
	${DEVIL_SRC_PATH}src-IL/src/il_exr.cpp \
	${DEVIL_SRC_PATH}src-IL/src/il_iwi.c \
	${DEVIL_SRC_PATH}src-IL/src/il_mng.c \
	${DEVIL_SRC_PATH}src-IL/src/il_rot.c \
	${DEVIL_SRC_PATH}src-IL/src/il_jpeg.c \
	${DEVIL_SRC_PATH}src-IL/src/il_png.c \
	${DEVIL_SRC_PATH}src-IL/src/il_pxr.c \
	${DEVIL_SRC_PATH}src-IL/src/il_nvidia.cpp \
	${DEVIL_SRC_PATH}src-IL/src/il_squish.cpp \
	${DEVIL_SRC_PATH}src-IL/src/il_utility.c \
	${DEVIL_SRC_PATH}src-IL/src/il_internal.c \
	${DEVIL_SRC_PATH}src-IL/src/il_psd.c \
	${DEVIL_SRC_PATH}src-IL/src/il_endian.c \
	${DEVIL_SRC_PATH}src-IL/src/il_quantizer.c \
	${DEVIL_SRC_PATH}src-IL/src/il_wbmp.c \
	${DEVIL_SRC_PATH}src-IL/src/il_manip.c \
	${DEVIL_SRC_PATH}src-IL/src/il_pic.c \
	${DEVIL_SRC_PATH}src-IL/src/il_tiff.c \
	${DEVIL_SRC_PATH}src-IL/src/il_pal.c \
	${DEVIL_SRC_PATH}src-IL/src/il_dds-save.c \
	${DEVIL_SRC_PATH}src-IL/src/il_raw.c \
	${DEVIL_SRC_PATH}src-IL/src/il_convert.c \
	${DEVIL_SRC_PATH}src-IL/src/il_dcx.c \
	${DEVIL_SRC_PATH}src-IL/src/il_fits.c \
	${DEVIL_SRC_PATH}src-IL/src/il_xpm.c \
	${DEVIL_SRC_PATH}src-IL/src/il_bits.c \
	${DEVIL_SRC_PATH}src-IL/src/altivec_common.c \
	${DEVIL_SRC_PATH}src-IL/src/il_dpx.c \
	${DEVIL_SRC_PATH}src-IL/src/il_rawdata.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := \
    libjasper \
    libpng \
    libmng \
    libjpeg \
    liblcms \
    libtiff

include $(BUILD_SHARED_LIBRARY)

#liblove
include $(CLEAR_VARS)

LOCAL_MODULE    := liblove
LOCAL_CFLAGS    := -g -Dlinux -DFT2_BUILD_LIBRARY=1 -DPHYSFS_NO_CDROM_SUPPORT=1 -DAL_ALEXT_PROTOTYPES=1 -DHAVE_GCC_DESTRUCTOR=1 -DOPT_GENERIC -DREAL_IS_FLOAT
LOCAL_CPPFLAGS  := ${LOCAL_CFLAGS}

LOCAL_C_INCLUDES  := \
	${LUA_SRC_PATH}src \
	${OPENAL_SRC_PATH}include/ \
	${OPENAL_SRC_PATH}OpenAL32/Include \
	${TIFF_SRC_PATH}libtiff/ \
	${MNG_SRC_PATH} \
	${LCMS_SRC_PATH}include/ \
	${PNG_SRC_PATH} \
	${JPEG_SRC_PATH} \
	${JASPER_SRC_PATH}src/libjasper/include \
	${DEVIL_SRC_PATH}include \
	${DEVIL_SRC_PATH}src-IL/include \
	${LOVE_SRC_PATH}src \
	${LOVE_SRC_PATH}src/modules \
	$(FREETYPE_SRC_PATH)include \
	$(FREETYPE_SRC_PATH)src \
	${PHYSFS_SRC_PATH} \
	${MPG123_SRC_PATH}src/libmpg123/ \
	${MODPLUG_SRC_PATH}src/ \
	${MODPLUG_SRC_PATH}src/libmodplug \
	${VORBIS_SRC_PATH}include \
	${OGG_SRC_PATH}/include
		
LOCAL_SRC_FILES := love.cpp \
	${LOVE_SRC_PATH}src/modules/sound/wrap_SoundData.cpp \
	${LOVE_SRC_PATH}src/modules/sound/Sound.cpp \
	${LOVE_SRC_PATH}src/modules/sound/lullaby/Decoder.cpp \
	${LOVE_SRC_PATH}src/modules/sound/lullaby/Mpg123Decoder.cpp \
	${LOVE_SRC_PATH}src/modules/sound/lullaby/Sound.cpp \
	${LOVE_SRC_PATH}src/modules/sound/lullaby/VorbisDecoder.cpp \
	${LOVE_SRC_PATH}src/modules/sound/lullaby/ModPlugDecoder.cpp \
	${LOVE_SRC_PATH}src/modules/sound/wrap_Sound.cpp \
	${LOVE_SRC_PATH}src/modules/sound/SoundData.cpp \
	${LOVE_SRC_PATH}src/modules/sound/wrap_Decoder.cpp \
	${LOVE_SRC_PATH}src/love.cpp \
	${LOVE_SRC_PATH}src/modules/event/sdl/wrap_Event.cpp \
	${LOVE_SRC_PATH}src/modules/event/sdl/Event.cpp \
	${LOVE_SRC_PATH}src/modules/event/Event.cpp \
	${LOVE_SRC_PATH}src/modules/audio/wrap_Source.cpp \
	${LOVE_SRC_PATH}src/modules/audio/wrap_Audio.cpp \
	${LOVE_SRC_PATH}src/modules/audio/openal/Source.cpp \
	${LOVE_SRC_PATH}src/modules/audio/openal/Pool.cpp \
	${LOVE_SRC_PATH}src/modules/audio/openal/Audio.cpp \
	${LOVE_SRC_PATH}src/modules/audio/null/Source.cpp \
	${LOVE_SRC_PATH}src/modules/audio/null/Audio.cpp \
	${LOVE_SRC_PATH}src/modules/audio/Source.cpp \
   	${LOVE_SRC_PATH}src/modules/timer/sdl/wrap_Timer.cpp \
	${LOVE_SRC_PATH}src/modules/timer/sdl/Timer.cpp \
	${LOVE_SRC_PATH}src/modules/thread/sdl/wrap_Thread.cpp \
	${LOVE_SRC_PATH}src/modules/thread/sdl/Thread.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/physfs/wrap_Filesystem.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/physfs/wrap_FileData.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/physfs/wrap_File.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/physfs/Filesystem.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/physfs/File.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/FileData.cpp \
	${LOVE_SRC_PATH}src/modules/filesystem/File.cpp \
	${LOVE_SRC_PATH}src/modules/font/wrap_Rasterizer.cpp \
	${LOVE_SRC_PATH}src/modules/font/wrap_GlyphData.cpp \
	${LOVE_SRC_PATH}src/modules/font/wrap_FontData.cpp \
	${LOVE_SRC_PATH}src/modules/font/freetype/wrap_Font.cpp \
	${LOVE_SRC_PATH}src/modules/font/freetype/TrueTypeRasterizer.cpp \
	${LOVE_SRC_PATH}src/modules/font/freetype/Font.cpp \
	${LOVE_SRC_PATH}src/modules/font/Rasterizer.cpp \
	${LOVE_SRC_PATH}src/modules/font/ImageRasterizer.cpp \
	${LOVE_SRC_PATH}src/modules/font/GlyphData.cpp \
	${LOVE_SRC_PATH}src/modules/font/FontData.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_World.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_Shape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_RevoluteJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_PulleyJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_PrismaticJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_PolygonShape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_Physics.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_MouseJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_Joint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_GearJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_DistanceJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_Contact.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_CircleShape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/wrap_Body.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/graham/GrahamScanConvexHull.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/World.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/b2WorldCallbacks.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/b2World.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/b2Island.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/b2ContactManager.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/b2Body.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2Joint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2GearJoint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Contacts/b2PolyContact.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Contacts/b2PolyAndCircleContact.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Contacts/b2Contact.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Common/b2StackAllocator.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Common/b2Settings.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Common/b2Math.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Common/b2BlockAllocator.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2TimeOfImpact.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2PairManager.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2Distance.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2Collision.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2CollidePoly.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2CollideCircle.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/b2BroadPhase.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/Shapes/b2Shape.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/Shapes/b2PolygonShape.cpp \
	${LOVE_SRC_PATH}src/libraries/Box2D/Collision/Shapes/b2CircleShape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/Shape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/RevoluteJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/PulleyJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/PrismaticJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/PolygonShape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/Physics.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/MouseJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/Joint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/GearJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/DistanceJoint.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/Contact.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/CircleShape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/box2d/Body.cpp \
	${LOVE_SRC_PATH}src/modules/physics/Shape.cpp \
	${LOVE_SRC_PATH}src/modules/physics/Joint.cpp \
	${LOVE_SRC_PATH}src/modules/mouse/sdl/wrap_Mouse.cpp \
	${LOVE_SRC_PATH}src/modules/mouse/sdl/Mouse.cpp \
	${LOVE_SRC_PATH}src/modules/mouse/Mouse.cpp \
	${LOVE_SRC_PATH}src/modules/keyboard/sdl/wrap_Keyboard.cpp \
	${LOVE_SRC_PATH}src/modules/keyboard/sdl/Keyboard.cpp \
	${LOVE_SRC_PATH}src/modules/keyboard/Keyboard.cpp \
	${LOVE_SRC_PATH}src/modules/image/wrap_ImageData.cpp \
	${LOVE_SRC_PATH}src/modules/image/wrap_Image.cpp \
	${LOVE_SRC_PATH}src/modules/image/wrap_EncodedImageData.cpp \
	${LOVE_SRC_PATH}src/modules/image/devil/ImageData.cpp \
	${LOVE_SRC_PATH}src/modules/image/devil/Image.cpp \
	${LOVE_SRC_PATH}src/modules/image/ImageData.cpp \
	${LOVE_SRC_PATH}src/modules/image/EncodedImageData.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_SpriteBatch.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_Quad.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_ParticleSystem.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_PixelEffect.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_Image.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_Graphics.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_Canvas.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/wrap_Font.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/SpriteBatch.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/Quad.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/ParticleSystem.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/PixelEffect.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/Image.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/Graphics.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/Glyph.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/Canvas.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/VertexBuffer.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/Font.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/opengl/OpenGL.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/Volatile.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/Image.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/Graphics.cpp \
	${LOVE_SRC_PATH}src/modules/graphics/Drawable.cpp \
	${LOVE_SRC_PATH}src/libraries/luasocket/luasocket.cpp \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/usocket.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/unix.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/udp.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/timeout.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/tcp.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/select.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/options.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/mime.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/luasocket.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/io.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/inet.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/except.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/buffer.c \
	${LOVE_SRC_PATH}src/libraries/luasocket/libluasocket/auxiliar.c \
	${LOVE_SRC_PATH}src/common/wrap_Data.cpp \
	${LOVE_SRC_PATH}src/common/utf8.cpp \
	${LOVE_SRC_PATH}src/common/runtime.cpp \
	${LOVE_SRC_PATH}src/common/b64.cpp \
	${LOVE_SRC_PATH}src/common/Vector.cpp \
	${LOVE_SRC_PATH}src/common/Reference.cpp \
	${LOVE_SRC_PATH}src/common/Object.cpp \
	${LOVE_SRC_PATH}src/common/MemoryData.cpp \
	${LOVE_SRC_PATH}src/common/Memoizer.cpp \
	${LOVE_SRC_PATH}src/common/Variant.cpp \
	${LOVE_SRC_PATH}src/common/Matrix.cpp \
	${LOVE_SRC_PATH}src/common/Exception.cpp
#	${LOVE_SRC_PATH}src/modules/joystick/sdl/wrap_Joystick.cpp \
#	${LOVE_SRC_PATH}src/modules/joystick/sdl/Joystick.cpp \
#	${LOVE_SRC_PATH}src/modules/joystick/Joystick.cpp \
#	${JPEG_SRC_PATH}jmemname.c \
#	${JPEG_SRC_PATH}jmemnobs.c \
#	${JASPER_SRC_PATH}src/libjasper/jpg/jpg_dummy.c \


LOCAL_LDLIBS    := -llog -L../lib -lGLESv2

LOCAL_STATIC_LIBRARIES := \
    libmpg123 \
    libvorbis \
    libogg \
    libmodplug \
    libopenal \
    libtiff \
    libjpeg \
    libphysfs \
    libmng \
    liblua \
    libfreetype \
    liblcms \
    libpng \
    libjasper \
    libdevil
  

include $(BUILD_SHARED_LIBRARY)
