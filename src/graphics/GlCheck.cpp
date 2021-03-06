#include <graphics/GlCheck.h>


#define STATUS_CASE(val) case val: return #val
const char* GlCheck::glStatusString(GLenum error)
{
  switch (error) {
    STATUS_CASE(GL_NO_ERROR);
    STATUS_CASE(GL_INVALID_ENUM);
    STATUS_CASE(GL_INVALID_VALUE);
    STATUS_CASE(GL_INVALID_OPERATION);
    STATUS_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
    STATUS_CASE(GL_OUT_OF_MEMORY);
    STATUS_CASE(GL_FRAMEBUFFER_COMPLETE);
    STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
    STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
    STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
    STATUS_CASE(GL_FRAMEBUFFER_UNSUPPORTED);
  }
  return NULL;
}
#undef STATUS_CASE

bool GlCheck::checkGlFramebuffer()
{
  Log log("CheckGL");
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_UNDEFINED)
      log.e() << "Framebuffer Status: Undefined. No window?" << Log::end;
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
      log.e() << "Framebuffer Status: Incomplete Attachment. "
              << "Check attachment status." << Log::end;
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
      log.e() << "Framebuffer Status: Incomplete Missing Attachment. "
              << "Attach at least one buffer to FBO" << Log::end;
#ifndef __ANDROID__
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
      log.e() << "Framebuffer Status: Incomplete Draw Buffer. "
              << "Check attachments enabled via glDrawBuffers are OK"
              << Log::end;
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
      log.e() << "Framebuffer Status: Incomplete Read Buffer. "
              << "Check attachments enabled via glReadBuffer are OK"
              << Log::end;
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
      log.e() << "Framebuffer Status: Incomplete Layer Targets. "
              << "Make sure number of layers is the same for each attachment"
              << Log::end;
#endif
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
      log.e() << "Framebuffer Status: Incomplete Multisample. "
              << "Make sure number of samples is the same for each attachment"
              << Log::end;
    else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
      log.e() << "Framebuffer Status: Unsupported. Formats might be bad."
              << Log::end;
    else
      log.e() << "Framebuffer Status: ??" << Log::end;

    log.exception("Framebuffer is not ok.");
    return false;
  }
  return true;
}

void GlCheck::debugGlFramebuffer() {
  Log log("CheckGL::checkGlFramebuffer");
    int nAttach = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,
                  &nAttach);

    for (GLenum i = 0u ; i < (uint32_t)nAttach ; ++i) {
      GLenum colorAttach = GL_COLOR_ATTACHMENT0 + i;

      int type;
      glGetFramebufferAttachmentParameteriv(
          GL_FRAMEBUFFER,
          colorAttach,
          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
          &type);

      if (type == GL_NONE)
        continue;
      log.e() << "Color attachment " << i << " "
               << GlCheck::glEnumString(type) << Log::end;

      if (type == GL_TEXTURE) {
        int rsize, gsize, bsize, asize;  //  , dsize, ssize;
        glGetFramebufferAttachmentParameteriv(
              GL_FRAMEBUFFER, colorAttach,
              GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
              &rsize);
        glGetFramebufferAttachmentParameteriv(
              GL_FRAMEBUFFER, colorAttach,
              GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
              &gsize);
        glGetFramebufferAttachmentParameteriv(
              GL_FRAMEBUFFER, colorAttach,
              GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
              &bsize);
        glGetFramebufferAttachmentParameteriv(
              GL_FRAMEBUFFER, colorAttach,
              GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
              &asize);

        log.w() << "RGBA depth: ["
                 << rsize << ", " << gsize << ", "
                 << bsize << ", " << asize << "]"
                 << Log::end;

        GLint textureId;
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            colorAttach,
            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
            &textureId);
        log.w() << "Texture id: " << textureId << Log::end;
      }
    }
    int type;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &type);
    log.e() << "Depth attachment " << GlCheck::glEnumString(type) << Log::end;
    int dsize;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
                                          &dsize);
    log.w() << "DEPTH depth: " << dsize << Log::end;


    // // TODO swarminglogic, 2014-07-24: This crashes android, why?
    // type = 0;
    // glGetFramebufferAttachmentParameteriv(
    //     GL_FRAMEBUFFER,
    //     GL_STENCIL_ATTACHMENT,
    //     GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
    //     &type);
    // log.e() << "Stencil attachment "
    //         << GlCheck::glEnumString(type) << Log::end;
    // int ssize;
    // glGetFramebufferAttachmentParameteriv(
    //     GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
    //     GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
    //     &ssize);
    // log.w() << "Stencil depth: " << ssize << Log::end;
}


#define STATUS_CASE(val) case val: return #val
const char* GlCheck::glEnumString(GLenum e)
{
  switch (e) {
    STATUS_CASE(GL_ACTIVE_ATTRIBUTES);
    STATUS_CASE(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
    STATUS_CASE(GL_ACTIVE_TEXTURE);
    STATUS_CASE(GL_ACTIVE_UNIFORMS);
    STATUS_CASE(GL_ACTIVE_UNIFORM_BLOCKS);
    STATUS_CASE(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
    STATUS_CASE(GL_ACTIVE_UNIFORM_MAX_LENGTH);
    STATUS_CASE(GL_ALIASED_LINE_WIDTH_RANGE);
    STATUS_CASE(GL_ALIASED_POINT_SIZE_RANGE);
    STATUS_CASE(GL_ALPHA);
    STATUS_CASE(GL_ALPHA_BITS);
    STATUS_CASE(GL_ALREADY_SIGNALED);
    STATUS_CASE(GL_ALWAYS);
    STATUS_CASE(GL_ANY_SAMPLES_PASSED);
    STATUS_CASE(GL_ANY_SAMPLES_PASSED_CONSERVATIVE);
    STATUS_CASE(GL_ARRAY_BUFFER);
    STATUS_CASE(GL_ARRAY_BUFFER_BINDING);
    STATUS_CASE(GL_ATTACHED_SHADERS);
    STATUS_CASE(GL_BACK);
    STATUS_CASE(GL_BLEND);
    STATUS_CASE(GL_BLEND_COLOR);
    STATUS_CASE(GL_BLEND_DST_ALPHA);
    STATUS_CASE(GL_BLEND_DST_RGB);
    STATUS_CASE(GL_BLEND_EQUATION_ALPHA);
    STATUS_CASE(GL_BLEND_EQUATION_RGB);
    STATUS_CASE(GL_BLEND_SRC_ALPHA);
    STATUS_CASE(GL_BLEND_SRC_RGB);
    STATUS_CASE(GL_BLUE);
    STATUS_CASE(GL_BLUE_BITS);
    STATUS_CASE(GL_BOOL);
    STATUS_CASE(GL_BOOL_VEC2);
    STATUS_CASE(GL_BOOL_VEC3);
    STATUS_CASE(GL_BOOL_VEC4);
    STATUS_CASE(GL_BUFFER_ACCESS_FLAGS);
    STATUS_CASE(GL_BUFFER_MAPPED);
    STATUS_CASE(GL_BUFFER_MAP_LENGTH);
    STATUS_CASE(GL_BUFFER_MAP_OFFSET);
    STATUS_CASE(GL_BUFFER_MAP_POINTER);
    STATUS_CASE(GL_BUFFER_SIZE);
    STATUS_CASE(GL_BUFFER_USAGE);
    STATUS_CASE(GL_BYTE);
    STATUS_CASE(GL_CCW);
    STATUS_CASE(GL_CLAMP_TO_EDGE);
    STATUS_CASE(GL_COLOR);
    STATUS_CASE(GL_COLOR_ATTACHMENT0);
    STATUS_CASE(GL_COLOR_ATTACHMENT1);
    STATUS_CASE(GL_COLOR_ATTACHMENT2);
    STATUS_CASE(GL_COLOR_ATTACHMENT3);
    STATUS_CASE(GL_COLOR_ATTACHMENT4);
    STATUS_CASE(GL_COLOR_ATTACHMENT5);
    STATUS_CASE(GL_COLOR_ATTACHMENT6);
    STATUS_CASE(GL_COLOR_ATTACHMENT7);
    STATUS_CASE(GL_COLOR_ATTACHMENT8);
    STATUS_CASE(GL_COLOR_ATTACHMENT9);
    STATUS_CASE(GL_COLOR_ATTACHMENT10);
    STATUS_CASE(GL_COLOR_ATTACHMENT11);
    STATUS_CASE(GL_COLOR_ATTACHMENT12);
    STATUS_CASE(GL_COLOR_ATTACHMENT13);
    STATUS_CASE(GL_COLOR_ATTACHMENT14);
    STATUS_CASE(GL_COLOR_ATTACHMENT15);
    STATUS_CASE(GL_COLOR_BUFFER_BIT);
    STATUS_CASE(GL_COLOR_CLEAR_VALUE);
    STATUS_CASE(GL_COLOR_WRITEMASK);
    STATUS_CASE(GL_COMPARE_REF_TO_TEXTURE);
    STATUS_CASE(GL_COMPILE_STATUS);
    STATUS_CASE(GL_COMPRESSED_R11_EAC);
    STATUS_CASE(GL_COMPRESSED_RG11_EAC);
    STATUS_CASE(GL_COMPRESSED_RGB8_ETC2);
    STATUS_CASE(GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2);
    STATUS_CASE(GL_COMPRESSED_RGBA8_ETC2_EAC);
    STATUS_CASE(GL_COMPRESSED_SIGNED_R11_EAC);
    STATUS_CASE(GL_COMPRESSED_SIGNED_RG11_EAC);
    STATUS_CASE(GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC);
    STATUS_CASE(GL_COMPRESSED_SRGB8_ETC2);
    STATUS_CASE(GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2);
    STATUS_CASE(GL_COMPRESSED_TEXTURE_FORMATS);
    STATUS_CASE(GL_CONDITION_SATISFIED);
    STATUS_CASE(GL_CONSTANT_ALPHA);
    STATUS_CASE(GL_CONSTANT_COLOR);
    STATUS_CASE(GL_COPY_READ_BUFFER);
    STATUS_CASE(GL_COPY_WRITE_BUFFER);
    STATUS_CASE(GL_CULL_FACE);
    STATUS_CASE(GL_CULL_FACE_MODE);
    STATUS_CASE(GL_CURRENT_PROGRAM);
    STATUS_CASE(GL_CURRENT_QUERY);
    STATUS_CASE(GL_CURRENT_VERTEX_ATTRIB);
    STATUS_CASE(GL_CW);
    STATUS_CASE(GL_DECR);
    STATUS_CASE(GL_DECR_WRAP);
    STATUS_CASE(GL_DELETE_STATUS);
    STATUS_CASE(GL_DEPTH);
    STATUS_CASE(GL_DEPTH24_STENCIL8);
    STATUS_CASE(GL_DEPTH32F_STENCIL8);
    STATUS_CASE(GL_DEPTH_ATTACHMENT);
    STATUS_CASE(GL_DEPTH_BITS);
    STATUS_CASE(GL_DEPTH_BUFFER_BIT);
    STATUS_CASE(GL_DEPTH_CLEAR_VALUE);
    STATUS_CASE(GL_DEPTH_COMPONENT);
    STATUS_CASE(GL_DEPTH_COMPONENT16);
    STATUS_CASE(GL_DEPTH_COMPONENT24);
    STATUS_CASE(GL_DEPTH_COMPONENT32F);
    STATUS_CASE(GL_DEPTH_FUNC);
    STATUS_CASE(GL_DEPTH_RANGE);
    STATUS_CASE(GL_DEPTH_STENCIL);
    STATUS_CASE(GL_DEPTH_STENCIL_ATTACHMENT);
    STATUS_CASE(GL_DEPTH_TEST);
    STATUS_CASE(GL_DEPTH_WRITEMASK);
    STATUS_CASE(GL_DITHER);
    STATUS_CASE(GL_DONT_CARE);
    STATUS_CASE(GL_DRAW_BUFFER0);
    STATUS_CASE(GL_DRAW_BUFFER1);
    STATUS_CASE(GL_DRAW_BUFFER10);
    STATUS_CASE(GL_DRAW_BUFFER11);
    STATUS_CASE(GL_DRAW_BUFFER12);
    STATUS_CASE(GL_DRAW_BUFFER13);
    STATUS_CASE(GL_DRAW_BUFFER14);
    STATUS_CASE(GL_DRAW_BUFFER15);
    STATUS_CASE(GL_DRAW_BUFFER2);
    STATUS_CASE(GL_DRAW_BUFFER3);
    STATUS_CASE(GL_DRAW_BUFFER4);
    STATUS_CASE(GL_DRAW_BUFFER5);
    STATUS_CASE(GL_DRAW_BUFFER6);
    STATUS_CASE(GL_DRAW_BUFFER7);
    STATUS_CASE(GL_DRAW_BUFFER8);
    STATUS_CASE(GL_DRAW_BUFFER9);
    STATUS_CASE(GL_DRAW_FRAMEBUFFER);
    STATUS_CASE(GL_DST_ALPHA);
    STATUS_CASE(GL_DST_COLOR);
    STATUS_CASE(GL_DYNAMIC_COPY);
    STATUS_CASE(GL_DYNAMIC_DRAW);
    STATUS_CASE(GL_DYNAMIC_READ);
    STATUS_CASE(GL_ELEMENT_ARRAY_BUFFER);
    STATUS_CASE(GL_ELEMENT_ARRAY_BUFFER_BINDING);
    STATUS_CASE(GL_EQUAL);
    STATUS_CASE(GL_EXTENSIONS);
    STATUS_CASE(GL_FASTEST);
    STATUS_CASE(GL_FIXED);
    STATUS_CASE(GL_FLOAT);
    STATUS_CASE(GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
    STATUS_CASE(GL_FLOAT_MAT2);
    STATUS_CASE(GL_FLOAT_MAT2x3);
    STATUS_CASE(GL_FLOAT_MAT2x4);
    STATUS_CASE(GL_FLOAT_MAT3);
    STATUS_CASE(GL_FLOAT_MAT3x2);
    STATUS_CASE(GL_FLOAT_MAT3x4);
    STATUS_CASE(GL_FLOAT_MAT4);
    STATUS_CASE(GL_FLOAT_MAT4x2);
    STATUS_CASE(GL_FLOAT_MAT4x3);
    STATUS_CASE(GL_FLOAT_VEC2);
    STATUS_CASE(GL_FLOAT_VEC3);
    STATUS_CASE(GL_FLOAT_VEC4);
    STATUS_CASE(GL_FRAGMENT_SHADER);
    STATUS_CASE(GL_FRAGMENT_SHADER_DERIVATIVE_HINT);
    STATUS_CASE(GL_FRAMEBUFFER);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER);
    STATUS_CASE(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
    STATUS_CASE(GL_FRAMEBUFFER_BINDING);
    STATUS_CASE(GL_FRAMEBUFFER_COMPLETE);
    STATUS_CASE(GL_FRAMEBUFFER_DEFAULT);
    STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
    STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
    STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
    STATUS_CASE(GL_FRAMEBUFFER_UNDEFINED);
    STATUS_CASE(GL_FRAMEBUFFER_UNSUPPORTED);
    STATUS_CASE(GL_FRONT);
    STATUS_CASE(GL_FRONT_AND_BACK);
    STATUS_CASE(GL_FRONT_FACE);
    STATUS_CASE(GL_FUNC_ADD);
    STATUS_CASE(GL_FUNC_REVERSE_SUBTRACT);
    STATUS_CASE(GL_FUNC_SUBTRACT);
    STATUS_CASE(GL_GENERATE_MIPMAP_HINT);
    STATUS_CASE(GL_GEQUAL);
    STATUS_CASE(GL_GREATER);
    STATUS_CASE(GL_GREEN);
    STATUS_CASE(GL_GREEN_BITS);
    STATUS_CASE(GL_HALF_FLOAT);
    STATUS_CASE(GL_HIGH_FLOAT);
    STATUS_CASE(GL_HIGH_INT);
    STATUS_CASE(GL_IMPLEMENTATION_COLOR_READ_FORMAT);
    STATUS_CASE(GL_IMPLEMENTATION_COLOR_READ_TYPE);
    STATUS_CASE(GL_INCR);
    STATUS_CASE(GL_INCR_WRAP);
    STATUS_CASE(GL_INFO_LOG_LENGTH);
    STATUS_CASE(GL_INT);
    STATUS_CASE(GL_INTERLEAVED_ATTRIBS);
    STATUS_CASE(GL_INT_2_10_10_10_REV);
    STATUS_CASE(GL_INT_SAMPLER_2D);
    STATUS_CASE(GL_INT_SAMPLER_2D_ARRAY);
    STATUS_CASE(GL_INT_SAMPLER_3D);
    STATUS_CASE(GL_INT_SAMPLER_CUBE);
    STATUS_CASE(GL_INT_VEC2);
    STATUS_CASE(GL_INT_VEC3);
    STATUS_CASE(GL_INT_VEC4);
    STATUS_CASE(GL_INVALID_ENUM);
    STATUS_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
    STATUS_CASE(GL_INVALID_OPERATION);
    STATUS_CASE(GL_INVALID_VALUE);
    STATUS_CASE(GL_INVERT);
    STATUS_CASE(GL_KEEP);
    STATUS_CASE(GL_LEQUAL);
    STATUS_CASE(GL_LESS);
    STATUS_CASE(GL_LINEAR);
    STATUS_CASE(GL_LINEAR_MIPMAP_LINEAR);
    STATUS_CASE(GL_LINEAR_MIPMAP_NEAREST);
    STATUS_CASE(GL_LINE_WIDTH);
    STATUS_CASE(GL_LINK_STATUS);
    STATUS_CASE(GL_LOW_FLOAT);
    STATUS_CASE(GL_LOW_INT);
    STATUS_CASE(GL_LUMINANCE);
    STATUS_CASE(GL_LUMINANCE_ALPHA);
    STATUS_CASE(GL_MAP_FLUSH_EXPLICIT_BIT);
    STATUS_CASE(GL_MAP_INVALIDATE_BUFFER_BIT);
    STATUS_CASE(GL_MAP_INVALIDATE_RANGE_BIT);
    STATUS_CASE(GL_MAP_READ_BIT);
    STATUS_CASE(GL_MAP_UNSYNCHRONIZED_BIT);
    STATUS_CASE(GL_MAP_WRITE_BIT);
    STATUS_CASE(GL_MAX);
    STATUS_CASE(GL_MAX_3D_TEXTURE_SIZE);
    STATUS_CASE(GL_MAX_ARRAY_TEXTURE_LAYERS);
    STATUS_CASE(GL_MAX_COLOR_ATTACHMENTS);
    STATUS_CASE(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
    STATUS_CASE(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    STATUS_CASE(GL_MAX_COMBINED_UNIFORM_BLOCKS);
    STATUS_CASE(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
    STATUS_CASE(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
    STATUS_CASE(GL_MAX_DRAW_BUFFERS);
    STATUS_CASE(GL_MAX_ELEMENTS_INDICES);
    STATUS_CASE(GL_MAX_ELEMENTS_VERTICES);
    STATUS_CASE(GL_MAX_ELEMENT_INDEX);
    STATUS_CASE(GL_MAX_FRAGMENT_INPUT_COMPONENTS);
    STATUS_CASE(GL_MAX_FRAGMENT_UNIFORM_BLOCKS);
    STATUS_CASE(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
    STATUS_CASE(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
    STATUS_CASE(GL_MAX_PROGRAM_TEXEL_OFFSET);
    STATUS_CASE(GL_MAX_RENDERBUFFER_SIZE);
    STATUS_CASE(GL_MAX_SAMPLES);
    STATUS_CASE(GL_MAX_SERVER_WAIT_TIMEOUT);
    STATUS_CASE(GL_MAX_TEXTURE_IMAGE_UNITS);
    STATUS_CASE(GL_MAX_TEXTURE_LOD_BIAS);
    STATUS_CASE(GL_MAX_TEXTURE_SIZE);
    STATUS_CASE(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS);
    STATUS_CASE(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS);
    STATUS_CASE(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS);
    STATUS_CASE(GL_MAX_UNIFORM_BLOCK_SIZE);
    STATUS_CASE(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    STATUS_CASE(GL_MAX_VARYING_COMPONENTS);
    STATUS_CASE(GL_MAX_VARYING_VECTORS);
    STATUS_CASE(GL_MAX_VERTEX_ATTRIBS);
    STATUS_CASE(GL_MAX_VERTEX_OUTPUT_COMPONENTS);
    STATUS_CASE(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    STATUS_CASE(GL_MAX_VERTEX_UNIFORM_BLOCKS);
    STATUS_CASE(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
    STATUS_CASE(GL_MAX_VERTEX_UNIFORM_VECTORS);
    STATUS_CASE(GL_MAX_VIEWPORT_DIMS);
    STATUS_CASE(GL_MEDIUM_FLOAT);
    STATUS_CASE(GL_MEDIUM_INT);
    STATUS_CASE(GL_MIN);
    STATUS_CASE(GL_MIN_PROGRAM_TEXEL_OFFSET);
    STATUS_CASE(GL_MIRRORED_REPEAT);
    STATUS_CASE(GL_NEAREST);
    STATUS_CASE(GL_NEAREST_MIPMAP_LINEAR);
    STATUS_CASE(GL_NEAREST_MIPMAP_NEAREST);
    STATUS_CASE(GL_NEVER);
    STATUS_CASE(GL_NICEST);
    STATUS_CASE(GL_NONE);
    STATUS_CASE(GL_NOTEQUAL);
    STATUS_CASE(GL_NUM_COMPRESSED_TEXTURE_FORMATS);
    STATUS_CASE(GL_NUM_EXTENSIONS);
    STATUS_CASE(GL_NUM_PROGRAM_BINARY_FORMATS);
    STATUS_CASE(GL_NUM_SAMPLE_COUNTS);
    STATUS_CASE(GL_NUM_SHADER_BINARY_FORMATS);
    STATUS_CASE(GL_OBJECT_TYPE);
    STATUS_CASE(GL_ONE_MINUS_CONSTANT_ALPHA);
    STATUS_CASE(GL_ONE_MINUS_CONSTANT_COLOR);
    STATUS_CASE(GL_ONE_MINUS_DST_ALPHA);
    STATUS_CASE(GL_ONE_MINUS_DST_COLOR);
    STATUS_CASE(GL_ONE_MINUS_SRC_ALPHA);
    STATUS_CASE(GL_ONE_MINUS_SRC_COLOR);
    STATUS_CASE(GL_OUT_OF_MEMORY);
    STATUS_CASE(GL_PACK_ALIGNMENT);
    STATUS_CASE(GL_PACK_ROW_LENGTH);
    STATUS_CASE(GL_PACK_SKIP_PIXELS);
    STATUS_CASE(GL_PACK_SKIP_ROWS);
    STATUS_CASE(GL_PIXEL_PACK_BUFFER);
    STATUS_CASE(GL_PIXEL_PACK_BUFFER_BINDING);
    STATUS_CASE(GL_PIXEL_UNPACK_BUFFER);
    STATUS_CASE(GL_PIXEL_UNPACK_BUFFER_BINDING);
    STATUS_CASE(GL_POLYGON_OFFSET_FACTOR);
    STATUS_CASE(GL_POLYGON_OFFSET_FILL);
    STATUS_CASE(GL_POLYGON_OFFSET_UNITS);
    STATUS_CASE(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    STATUS_CASE(GL_PROGRAM_BINARY_FORMATS);
    STATUS_CASE(GL_PROGRAM_BINARY_LENGTH);
    STATUS_CASE(GL_PROGRAM_BINARY_RETRIEVABLE_HINT);
    STATUS_CASE(GL_QUERY_RESULT);
    STATUS_CASE(GL_QUERY_RESULT_AVAILABLE);
    STATUS_CASE(GL_R11F_G11F_B10F);
    STATUS_CASE(GL_R16F);
    STATUS_CASE(GL_R16I);
    STATUS_CASE(GL_R16UI);
    STATUS_CASE(GL_R32F);
    STATUS_CASE(GL_R32I);
    STATUS_CASE(GL_R32UI);
    STATUS_CASE(GL_R8);
    STATUS_CASE(GL_R8I);
    STATUS_CASE(GL_R8UI);
    STATUS_CASE(GL_R8_SNORM);
    STATUS_CASE(GL_RASTERIZER_DISCARD);
    STATUS_CASE(GL_READ_BUFFER);
    STATUS_CASE(GL_READ_FRAMEBUFFER);
    STATUS_CASE(GL_READ_FRAMEBUFFER_BINDING);
    STATUS_CASE(GL_RED);
    STATUS_CASE(GL_RED_BITS);
    STATUS_CASE(GL_RED_INTEGER);
    STATUS_CASE(GL_RENDERBUFFER);
    STATUS_CASE(GL_RENDERBUFFER_ALPHA_SIZE);
    STATUS_CASE(GL_RENDERBUFFER_BINDING);
    STATUS_CASE(GL_RENDERBUFFER_BLUE_SIZE);
    STATUS_CASE(GL_RENDERBUFFER_DEPTH_SIZE);
    STATUS_CASE(GL_RENDERBUFFER_GREEN_SIZE);
    STATUS_CASE(GL_RENDERBUFFER_HEIGHT);
    STATUS_CASE(GL_RENDERBUFFER_INTERNAL_FORMAT);
    STATUS_CASE(GL_RENDERBUFFER_RED_SIZE);
    STATUS_CASE(GL_RENDERBUFFER_SAMPLES);
    STATUS_CASE(GL_RENDERBUFFER_STENCIL_SIZE);
    STATUS_CASE(GL_RENDERBUFFER_WIDTH);
    STATUS_CASE(GL_RENDERER);
    STATUS_CASE(GL_REPEAT);
    STATUS_CASE(GL_REPLACE);
    STATUS_CASE(GL_RG);
    STATUS_CASE(GL_RG16F);
    STATUS_CASE(GL_RG16I);
    STATUS_CASE(GL_RG16UI);
    STATUS_CASE(GL_RG32F);
    STATUS_CASE(GL_RG32I);
    STATUS_CASE(GL_RG32UI);
    STATUS_CASE(GL_RG8);
    STATUS_CASE(GL_RG8I);
    STATUS_CASE(GL_RG8UI);
    STATUS_CASE(GL_RG8_SNORM);
    STATUS_CASE(GL_RGB);
    STATUS_CASE(GL_RGB10_A2);
    STATUS_CASE(GL_RGB10_A2UI);
    STATUS_CASE(GL_RGB16F);
    STATUS_CASE(GL_RGB16I);
    STATUS_CASE(GL_RGB16UI);
    STATUS_CASE(GL_RGB32F);
    STATUS_CASE(GL_RGB32I);
    STATUS_CASE(GL_RGB32UI);
    STATUS_CASE(GL_RGB565);
    STATUS_CASE(GL_RGB5_A1);
    STATUS_CASE(GL_RGB8);
    STATUS_CASE(GL_RGB8I);
    STATUS_CASE(GL_RGB8UI);
    STATUS_CASE(GL_RGB8_SNORM);
    STATUS_CASE(GL_RGB9_E5);
    STATUS_CASE(GL_RGBA);
    STATUS_CASE(GL_RGBA16F);
    STATUS_CASE(GL_RGBA16I);
    STATUS_CASE(GL_RGBA16UI);
    STATUS_CASE(GL_RGBA32F);
    STATUS_CASE(GL_RGBA32I);
    STATUS_CASE(GL_RGBA32UI);
    STATUS_CASE(GL_RGBA4);
    STATUS_CASE(GL_RGBA8);
    STATUS_CASE(GL_RGBA8I);
    STATUS_CASE(GL_RGBA8UI);
    STATUS_CASE(GL_RGBA8_SNORM);
    STATUS_CASE(GL_RGBA_INTEGER);
    STATUS_CASE(GL_RGB_INTEGER);
    STATUS_CASE(GL_RG_INTEGER);
    STATUS_CASE(GL_SAMPLER_2D);
    STATUS_CASE(GL_SAMPLER_2D_ARRAY);
    STATUS_CASE(GL_SAMPLER_2D_ARRAY_SHADOW);
    STATUS_CASE(GL_SAMPLER_2D_SHADOW);
    STATUS_CASE(GL_SAMPLER_3D);
    STATUS_CASE(GL_SAMPLER_BINDING);
    STATUS_CASE(GL_SAMPLER_CUBE);
    STATUS_CASE(GL_SAMPLER_CUBE_SHADOW);
    STATUS_CASE(GL_SAMPLES);
    STATUS_CASE(GL_SAMPLE_ALPHA_TO_COVERAGE);
    STATUS_CASE(GL_SAMPLE_BUFFERS);
    STATUS_CASE(GL_SAMPLE_COVERAGE);
    STATUS_CASE(GL_SAMPLE_COVERAGE_INVERT);
    STATUS_CASE(GL_SAMPLE_COVERAGE_VALUE);
    STATUS_CASE(GL_SCISSOR_BOX);
    STATUS_CASE(GL_SCISSOR_TEST);
    STATUS_CASE(GL_SEPARATE_ATTRIBS);
    STATUS_CASE(GL_SHADER_BINARY_FORMATS);
    STATUS_CASE(GL_SHADER_COMPILER);
    STATUS_CASE(GL_SHADER_SOURCE_LENGTH);
    STATUS_CASE(GL_SHADER_TYPE);
    STATUS_CASE(GL_SHORT);
    STATUS_CASE(GL_SIGNALED);
    STATUS_CASE(GL_SIGNED_NORMALIZED);
    STATUS_CASE(GL_SRC_ALPHA);
    STATUS_CASE(GL_SRC_ALPHA_SATURATE);
    STATUS_CASE(GL_SRC_COLOR);
    STATUS_CASE(GL_SRGB);
    STATUS_CASE(GL_SRGB8);
    STATUS_CASE(GL_SRGB8_ALPHA8);
    STATUS_CASE(GL_STATIC_COPY);
    STATUS_CASE(GL_STATIC_DRAW);
    STATUS_CASE(GL_STATIC_READ);
    STATUS_CASE(GL_STENCIL);
    STATUS_CASE(GL_STENCIL_ATTACHMENT);
    STATUS_CASE(GL_STENCIL_BACK_FAIL);
    STATUS_CASE(GL_STENCIL_BACK_FUNC);
    STATUS_CASE(GL_STENCIL_BACK_PASS_DEPTH_FAIL);
    STATUS_CASE(GL_STENCIL_BACK_PASS_DEPTH_PASS);
    STATUS_CASE(GL_STENCIL_BACK_REF);
    STATUS_CASE(GL_STENCIL_BACK_VALUE_MASK);
    STATUS_CASE(GL_STENCIL_BACK_WRITEMASK);
    STATUS_CASE(GL_STENCIL_BITS);
    STATUS_CASE(GL_STENCIL_BUFFER_BIT);
    STATUS_CASE(GL_STENCIL_CLEAR_VALUE);
    STATUS_CASE(GL_STENCIL_FAIL);
    STATUS_CASE(GL_STENCIL_FUNC);
    STATUS_CASE(GL_STENCIL_INDEX8);
    STATUS_CASE(GL_STENCIL_PASS_DEPTH_FAIL);
    STATUS_CASE(GL_STENCIL_PASS_DEPTH_PASS);
    STATUS_CASE(GL_STENCIL_REF);
    STATUS_CASE(GL_STENCIL_TEST);
    STATUS_CASE(GL_STENCIL_VALUE_MASK);
    STATUS_CASE(GL_STENCIL_WRITEMASK);
    STATUS_CASE(GL_STREAM_COPY);
    STATUS_CASE(GL_STREAM_DRAW);
    STATUS_CASE(GL_STREAM_READ);
    STATUS_CASE(GL_SUBPIXEL_BITS);
    STATUS_CASE(GL_SYNC_CONDITION);
    STATUS_CASE(GL_SYNC_FENCE);
    STATUS_CASE(GL_SYNC_FLAGS);
    STATUS_CASE(GL_SYNC_GPU_COMMANDS_COMPLETE);
    STATUS_CASE(GL_SYNC_STATUS);
    STATUS_CASE(GL_TEXTURE);
    STATUS_CASE(GL_TEXTURE0);
    STATUS_CASE(GL_TEXTURE1);
    STATUS_CASE(GL_TEXTURE10);
    STATUS_CASE(GL_TEXTURE11);
    STATUS_CASE(GL_TEXTURE12);
    STATUS_CASE(GL_TEXTURE13);
    STATUS_CASE(GL_TEXTURE14);
    STATUS_CASE(GL_TEXTURE15);
    STATUS_CASE(GL_TEXTURE16);
    STATUS_CASE(GL_TEXTURE17);
    STATUS_CASE(GL_TEXTURE18);
    STATUS_CASE(GL_TEXTURE19);
    STATUS_CASE(GL_TEXTURE2);
    STATUS_CASE(GL_TEXTURE20);
    STATUS_CASE(GL_TEXTURE21);
    STATUS_CASE(GL_TEXTURE22);
    STATUS_CASE(GL_TEXTURE23);
    STATUS_CASE(GL_TEXTURE24);
    STATUS_CASE(GL_TEXTURE25);
    STATUS_CASE(GL_TEXTURE26);
    STATUS_CASE(GL_TEXTURE27);
    STATUS_CASE(GL_TEXTURE28);
    STATUS_CASE(GL_TEXTURE29);
    STATUS_CASE(GL_TEXTURE3);
    STATUS_CASE(GL_TEXTURE30);
    STATUS_CASE(GL_TEXTURE31);
    STATUS_CASE(GL_TEXTURE4);
    STATUS_CASE(GL_TEXTURE5);
    STATUS_CASE(GL_TEXTURE6);
    STATUS_CASE(GL_TEXTURE7);
    STATUS_CASE(GL_TEXTURE8);
    STATUS_CASE(GL_TEXTURE9);
    STATUS_CASE(GL_TEXTURE_2D);
    STATUS_CASE(GL_TEXTURE_2D_ARRAY);
    STATUS_CASE(GL_TEXTURE_3D);
    STATUS_CASE(GL_TEXTURE_BASE_LEVEL);
    STATUS_CASE(GL_TEXTURE_BINDING_2D);
    STATUS_CASE(GL_TEXTURE_BINDING_2D_ARRAY);
    STATUS_CASE(GL_TEXTURE_BINDING_3D);
    STATUS_CASE(GL_TEXTURE_BINDING_CUBE_MAP);
    STATUS_CASE(GL_TEXTURE_COMPARE_FUNC);
    STATUS_CASE(GL_TEXTURE_COMPARE_MODE);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    STATUS_CASE(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    STATUS_CASE(GL_TEXTURE_IMMUTABLE_FORMAT);
    STATUS_CASE(GL_TEXTURE_IMMUTABLE_LEVELS);
    STATUS_CASE(GL_TEXTURE_MAG_FILTER);
    STATUS_CASE(GL_TEXTURE_MAX_LEVEL);
    STATUS_CASE(GL_TEXTURE_MAX_LOD);
    STATUS_CASE(GL_TEXTURE_MIN_FILTER);
    STATUS_CASE(GL_TEXTURE_MIN_LOD);
    STATUS_CASE(GL_TEXTURE_SWIZZLE_A);
    STATUS_CASE(GL_TEXTURE_SWIZZLE_B);
    STATUS_CASE(GL_TEXTURE_SWIZZLE_G);
    STATUS_CASE(GL_TEXTURE_SWIZZLE_R);
    STATUS_CASE(GL_TEXTURE_WRAP_R);
    STATUS_CASE(GL_TEXTURE_WRAP_S);
    STATUS_CASE(GL_TEXTURE_WRAP_T);
    STATUS_CASE(GL_TIMEOUT_EXPIRED);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK);
    // STATUS_CASE(GL_TRANSFORM_FEEDBACK_ACTIVE);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_BINDING);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_BUFFER);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_BUFFER_MODE);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_BUFFER_START);
    // STATUS_CASE(GL_TRANSFORM_FEEDBACK_PAUSED);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_VARYINGS);
    STATUS_CASE(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH);
    STATUS_CASE(GL_UNIFORM_ARRAY_STRIDE);
    STATUS_CASE(GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS);
    STATUS_CASE(GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES);
    STATUS_CASE(GL_UNIFORM_BLOCK_BINDING);
    STATUS_CASE(GL_UNIFORM_BLOCK_DATA_SIZE);
    STATUS_CASE(GL_UNIFORM_BLOCK_INDEX);
    STATUS_CASE(GL_UNIFORM_BLOCK_NAME_LENGTH);
    STATUS_CASE(GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER);
    STATUS_CASE(GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER);
    STATUS_CASE(GL_UNIFORM_BUFFER);
    STATUS_CASE(GL_UNIFORM_BUFFER_BINDING);
    STATUS_CASE(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT);
    STATUS_CASE(GL_UNIFORM_BUFFER_SIZE);
    STATUS_CASE(GL_UNIFORM_BUFFER_START);
    STATUS_CASE(GL_UNIFORM_IS_ROW_MAJOR);
    STATUS_CASE(GL_UNIFORM_MATRIX_STRIDE);
    STATUS_CASE(GL_UNIFORM_NAME_LENGTH);
    STATUS_CASE(GL_UNIFORM_OFFSET);
    STATUS_CASE(GL_UNIFORM_SIZE);
    STATUS_CASE(GL_UNIFORM_TYPE);
    STATUS_CASE(GL_UNPACK_ALIGNMENT);
    STATUS_CASE(GL_UNPACK_IMAGE_HEIGHT);
    STATUS_CASE(GL_UNPACK_ROW_LENGTH);
    STATUS_CASE(GL_UNPACK_SKIP_IMAGES);
    STATUS_CASE(GL_UNPACK_SKIP_PIXELS);
    STATUS_CASE(GL_UNPACK_SKIP_ROWS);
    STATUS_CASE(GL_UNSIGNALED);
    STATUS_CASE(GL_UNSIGNED_BYTE);
    STATUS_CASE(GL_UNSIGNED_INT);
    STATUS_CASE(GL_UNSIGNED_INT_10F_11F_11F_REV);
    STATUS_CASE(GL_UNSIGNED_INT_24_8);
    STATUS_CASE(GL_UNSIGNED_INT_2_10_10_10_REV);
    STATUS_CASE(GL_UNSIGNED_INT_5_9_9_9_REV);
    STATUS_CASE(GL_UNSIGNED_INT_SAMPLER_2D);
    STATUS_CASE(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY);
    STATUS_CASE(GL_UNSIGNED_INT_SAMPLER_3D);
    STATUS_CASE(GL_UNSIGNED_INT_SAMPLER_CUBE);
    STATUS_CASE(GL_UNSIGNED_INT_VEC2);
    STATUS_CASE(GL_UNSIGNED_INT_VEC3);
    STATUS_CASE(GL_UNSIGNED_INT_VEC4);
    STATUS_CASE(GL_UNSIGNED_NORMALIZED);
    STATUS_CASE(GL_UNSIGNED_SHORT);
    STATUS_CASE(GL_UNSIGNED_SHORT_4_4_4_4);
    STATUS_CASE(GL_UNSIGNED_SHORT_5_5_5_1);
    STATUS_CASE(GL_UNSIGNED_SHORT_5_6_5);
    STATUS_CASE(GL_VALIDATE_STATUS);
    STATUS_CASE(GL_VENDOR);
    STATUS_CASE(GL_VERTEX_ARRAY_BINDING);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_DIVISOR);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_ENABLED);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_INTEGER);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_POINTER);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_SIZE);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_STRIDE);
    STATUS_CASE(GL_VERTEX_ATTRIB_ARRAY_TYPE);
    STATUS_CASE(GL_VERTEX_SHADER);
    STATUS_CASE(GL_VIEWPORT);
    STATUS_CASE(GL_WAIT_FAILED);
  }
  return NULL;
}
#undef STATUS_CASE
