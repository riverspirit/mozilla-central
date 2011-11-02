# shared libs for webrtc
SHARED_LIBRARY_LIBS += \
        $(MKSHLIB_FORCE_ALL) \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/third_party_mods/libjingle/libjingle.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/third_party_mods/libjingle/libjingle_p2p.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/third_party_mods/libjingle/libjingle_app.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/third_party_mods/libjingle/libjsoncpp.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libvideo_capture_module.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libwebrtc_utility.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaudio_coding_module.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libCNG.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/common_audio/libspl.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libG711.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libG722.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libiLBC.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libiSAC.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libiSACFix.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libPCM16B.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libNetEq.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/common_audio/libresampler.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/common_audio/libvad.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/system_wrappers/source/libsystem_wrappers.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libwebrtc_video_coding.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libwebrtc_i420.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libwebrtc_vp8.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/common_video/libwebrtc_vplib.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libvideo_render_module.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/video_engine/libvideo_engine_core.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libmedia_file.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/librtp_rtcp.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libudp_transport.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libvideo_processing.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/voice_engine/libvoice_engine_core.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaudio_conference_mixer.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaudio_device.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaudio_processing.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaec.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libapm_util.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaecm.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libagc.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libns.a \
        $(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/modules/libaudioproc_debug_proto.a \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/third_party/protobuf/libprotobuf_lite.a \
        $(MKSHLIB_UNFORCE_ALL) \
	$(DEPTH)/media/webrtc/trunk/out/out/Debug/obj.target/src/common_video/libwebrtc_jpeg.a \
	-lexpat \
	$(NULL)
