#include <gst/gst.h>
static void cb_new_pad(GstElement *decoder,GstPad *pad,gpointer sink);
int main(int argc, char *argv[]) {
  GstElement *pipeline, *source, *decoder, *sink;
  GstBus *bus;
  GstMessage *msg;
  GstStateChangeReturn ret;

  /* Initialize GStreamer */
  gst_init(&argc, &argv);

  /* Create pipeline elements */
  pipeline = gst_pipeline_new("video-decoder");

  /* create elements */
  source = gst_element_factory_make("filesrc", "file-source");
  decoder = gst_element_factory_make("decodebin", "decoder");
  sink = gst_element_factory_make("autovideosink", "video-output");

  /* Setting file path to source */
  g_object_set(G_OBJECT(source), "location", "sample.ts", NULL);

  /* add elements to pipeline */
  gst_bin_add_many(GST_BIN(pipeline), source, decoder, sink, NULL);

  /* linking source and decoder and checking */
  if (!gst_element_link(source, decoder)) {
    g_printerr("Failed to link source and decoder\n");
    return -1;
  }

  /* Connect to decodebin's "pad-added" signal */
  g_signal_connect(decoder, "pad-added", G_CALLBACK(cb_new_pad), sink);

  /* set pipeline to playing state */
  ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to start up pipeline\n");
    return -1;
  }

  /* Waiting for EOS */
  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);

  /* dereferencing */
  gst_message_unref(msg);
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}

/* Callback function to link decodebin's dynamic pad to the sink */
static void cb_new_pad(GstElement *decoder, GstPad *pad, gpointer sink) {
  GstPad *sink_pad = gst_element_get_static_pad((GstElement *) sink, "sink");
  GstPadLinkReturn ret;

  /* Link the decoder pad to the sink pad */
  ret = gst_pad_link(pad, sink_pad);
  if (GST_PAD_LINK_FAILED(ret)) {
    g_printerr("Failed to link decoder and sink\n");
  }

  gst_object_unref(sink_pad);
}
