/* Hacky Demo App. Does some OpenGL computation and draws onto the screen. */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static const char *g_text = "FOSDEM";
static const char *g_subtitle = "in a CCA Realm VM";
static void *g_font = GLUT_STROKE_ROMAN;
static void *g_font2 = GLUT_STROKE_MONO_ROMAN;

static int   g_w = 900, g_h = 600;
static float g_pos_x = 0.0f, g_pos_y = 0.0f;
static float g_vel_x = 0.9f, g_vel_y = 0.7f;

static float g_rot_y = 0.0f, g_rot_x = 0.0f;

static int   g_last_ms = 0;

static int   g_frames = 0;
static int   g_fps_last_ms = 0;
static float g_fps = 0.0f;

static const char *g_gl_vendor   = NULL;
static const char *g_gl_renderer = NULL;
static const char *g_gl_version  = NULL;
static const char *g_glsl_ver    = NULL;

static GLuint g_char_lists[64];
static float  g_char_widths[64];
static int    g_char_count = 0;

static void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b) {
    h = fmodf(h, 1.0f);
    if (h < 0.0f) h += 1.0f;

    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h * 6.0f, 2.0f) - 1.0f));
    float m = v - c;

    float rp=0, gp=0, bp=0;
    if      (h < 1.0f/6.0f) { rp=c; gp=x; bp=0; }
    else if (h < 2.0f/6.0f) { rp=x; gp=c; bp=0; }
    else if (h < 3.0f/6.0f) { rp=0; gp=c; bp=x; }
    else if (h < 4.0f/6.0f) { rp=0; gp=x; bp=c; }
    else if (h < 5.0f/6.0f) { rp=x; gp=0; bp=c; }
    else                    { rp=c; gp=0; bp=x; }

    *r = rp + m; *g = gp + m; *b = bp + m;
}

static float stroke_text_width(void *font, const char *s) {
    float w = 0.0f;
    for (const unsigned char *p = (const unsigned char*)s; *p; ++p) {
        w += (float)glutStrokeWidth(font, *p);
    }
    return w;
}

static void build_char_display_lists(void) {
    g_char_count = (int)strlen(g_text);
    if (g_char_count > 64) g_char_count = 64;

    GLuint base = glGenLists((GLsizei)g_char_count);
    for (int i = 0; i < g_char_count; ++i) {
        unsigned char c = (unsigned char)g_text[i];
        g_char_lists[i] = base + (GLuint)i;
        g_char_widths[i] = (float)glutStrokeWidth(g_font, c);

        glNewList(g_char_lists[i], GL_COMPILE);
        glutStrokeCharacter(g_font, c);
        glEndList();
    }
}

static void draw_bitmap_line(int x, int y, void *font, const char *s) {
    glRasterPos2i(x, y);
    for (const char *p = s; *p; ++p) glutBitmapCharacter(font, *p);
}

static void draw_overlay_2d(void) {
    const int pad = 15;
    void *font = GLUT_BITMAP_9_BY_15;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, g_w, 0, g_h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    char line1[128];
    snprintf(line1, sizeof(line1), "FPS: %.1f", g_fps);

    const char *rend = g_gl_renderer ? g_gl_renderer : "unknown renderer";
    const char *vend = g_gl_vendor   ? g_gl_vendor   : "unknown vendor";
    const char *ver  = g_gl_version  ? g_gl_version  : "unknown version";
    const char *glsl = g_glsl_ver    ? g_glsl_ver    : "unknown GLSL";

    char line2[256];
    snprintf(line2, sizeof(line2), "GPU: %.160s", rend);

    char line3[256];
    snprintf(line3, sizeof(line3), "Vendor: %.160s", vend);

    char line4[256];
    snprintf(line4, sizeof(line4), "OpenGL: %.160s", ver);

    char line5[256];
    snprintf(line5, sizeof(line5), "GLSL: %.160s", glsl);

    glColor3f(0.95f, 0.95f, 0.98f);

    int x = pad;
    int y = g_h - pad - 18;
    const int line_step = 20;

    draw_bitmap_line(x, y, font, line1); y -= line_step;
    draw_bitmap_line(x, y, font, line2); y -= line_step;
    draw_bitmap_line(x, y, font, line3); y -= line_step;
    draw_bitmap_line(x, y, font, line4); y -= line_step;
    draw_bitmap_line(x, y, font, line5); y -= line_step;

    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

static void update_window_title(void) {
    char title[512];
    const char *rend = g_gl_renderer ? g_gl_renderer : "unknown GPU";

    char short_rend[128];
    snprintf(short_rend, sizeof(short_rend), "%.110s", rend);

    snprintf(title, sizeof(title), "FOSDEM | %.1f FPS | %s", g_fps, short_rend);
    glutSetWindowTitle(title);
}

static void draw_text_sliced_3d_optimized(float tsec) {
    const float w = stroke_text_width(g_font, g_text);

    const float target_world_width = 5.8f;
    const float scale = (w > 0.0f) ? (target_world_width / w) : 0.01f;

    const int   slices = 72;
    const float dz     = 0.027f;

    const float pulse = 0.88f + 0.12f * sinf(tsec * 2.2f);

    glPushMatrix();
    glScalef(scale, scale, scale);
    glTranslatef(-0.5f * w, -95.0f, 0.0f);

    for (int i = slices - 1; i >= 0; --i) {
        float zt = (float)i / (float)(slices - 1);

        float depth_dim = 1.0f - 0.75f * zt;
        float depth_tint = 0.06f * zt;

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -(float)i * (dz / scale));

        float x_cursor = 0.0f;
        for (int idx = 0; idx < g_char_count; ++idx) {
            float along = (g_char_count > 1) ? ((float)idx / (float)(g_char_count - 1)) : 0.0f;
            float hue = along * 0.85f + 0.10f * sinf(tsec * 0.7f);

            float r, g, b;
            hsv_to_rgb(hue, 0.95f, 1.0f, &r, &g, &b);

            r = (r * pulse * depth_dim) + depth_tint;
            g = (g * pulse * depth_dim);
            b = (b * pulse * depth_dim) + (0.02f * (1.0f - zt));

            if (r > 1.0f) r = 1.0f;
            if (g > 1.0f) g = 1.0f;
            if (b > 1.0f) b = 1.0f;

            glColor3f(r, g, b);

            glPushMatrix();
            glTranslatef(x_cursor, 0.0f, 0.0f);
            glCallList(g_char_lists[idx]);
            glPopMatrix();

            x_cursor += g_char_widths[idx];
        }

        glPopMatrix();
    }

    glPopMatrix();
}


static void draw_stroke_string_spaced(void *font, const char *s, float extra_per_char) {
    for (const unsigned char *p = (const unsigned char*)s; *p; ++p) {
        glutStrokeCharacter(font, *p);

        float adv = (float)glutStrokeWidth(font, *p);
        if (p[1]) adv += extra_per_char;

        glTranslatef(adv, 0.0f, 0.0f);
    }
}

static void draw_subtitle_3d(float tsec) {
    const float w_main = stroke_text_width(g_font, g_text);
    const float w_sub  = stroke_text_width(g_font2, g_subtitle);
    if (w_main <= 0.0f || w_sub <= 0.0f) return;

    const float extra_eff = 6.0f;

    const float target_world_width = 5.8f;
    const float scale_main = target_world_width / w_main;

    float sub_scale = 0.26f;

    const float fit_scale = 0.92f * (w_main / w_sub);
    if (fit_scale < sub_scale) sub_scale = fit_scale;

    if (sub_scale < 0.14f) sub_scale = 0.14f;

    const float pulse = 0.92f + 0.08f * sinf(tsec * 2.0f);

    glPushMatrix();

    glScalef(scale_main, scale_main, scale_main);
    glTranslatef(-0.5f * w_main, -95.0f, 0.0f);

    glScalef(sub_scale, sub_scale, sub_scale);


    const float x_eff = 15.0f;
    const float y_eff = -35.0f;

    const float x = x_eff / sub_scale;
    const float y = y_eff / sub_scale;

    const float z_eff = 0.06f / scale_main;
    const float z = z_eff / sub_scale;

    glTranslatef(x, y, z);

    glColor3f(0.92f * pulse, 0.92f * pulse, 0.96f * pulse);

    for (const unsigned char *p = (const unsigned char*)g_subtitle; *p; ++p) {
        glutStrokeCharacter(g_font2, *p);
    }

    glPopMatrix();
}

static void reshape(int w, int h) {
    g_w = (w > 1) ? w : 1;
    g_h = (h > 1) ? h : 1;
    glViewport(0, 0, g_w, g_h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0, (double)g_w / (double)g_h, 0.1, 120.0);

    glMatrixMode(GL_MODELVIEW);
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        0.0, 0.0, 6.2,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    glTranslatef(g_pos_x, g_pos_y, 0.0f);
    glRotatef(g_rot_y, 0.0f, 1.0f, 0.0f);
    glRotatef(g_rot_x, 1.0f, 0.0f, 0.0f);

    float tsec = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    draw_text_sliced_3d_optimized(tsec);
    draw_subtitle_3d(tsec);

    draw_overlay_2d();

    glutSwapBuffers();

    g_frames++;
    int now = glutGet(GLUT_ELAPSED_TIME);
    if (g_fps_last_ms == 0) g_fps_last_ms = now;
    int elapsed = now - g_fps_last_ms;
    if (elapsed >= 1000) {
        g_fps = (float)g_frames * 1000.0f / (float)elapsed;
        g_frames = 0;
        g_fps_last_ms = now;
        update_window_title();
    }
}

static void tick(void) {
    int now = glutGet(GLUT_ELAPSED_TIME);
    int dt_ms = now - g_last_ms;
    if (dt_ms < 1) dt_ms = 1;
    g_last_ms = now;

    float dt = (float)dt_ms / 1000.0f;

    const float x_min = -1.6f, x_max = 1.6f;
    const float y_min = -1.0f, y_max = 1.0f;

    g_pos_x += g_vel_x * dt;
    g_pos_y += g_vel_y * dt;

    if (g_pos_x < x_min) { g_pos_x = x_min; g_vel_x = fabsf(g_vel_x); }
    if (g_pos_x > x_max) { g_pos_x = x_max; g_vel_x = -fabsf(g_vel_x); }
    if (g_pos_y < y_min) { g_pos_y = y_min; g_vel_y = fabsf(g_vel_y); }
    if (g_pos_y > y_max) { g_pos_y = y_max; g_vel_y = -fabsf(g_vel_y); }

    float tsec = (float)now / 1000.0f;
    g_rot_y = 8.0f * sinf(tsec * 0.9f);
    g_rot_x = 0.0f;

    glutPostRedisplay();
}

static void timer_cb(int v) {
    (void)v;
    tick();
    glutTimerFunc(16, timer_cb, 0);
}

static void init_gl(void) {
    glClearColor(0.01f, 0.01f, 0.03f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDisable(GL_LIGHTING);

    glDisable(GL_LINE_SMOOTH);

    g_gl_vendor   = (const char*)glGetString(GL_VENDOR);
    g_gl_renderer = (const char*)glGetString(GL_RENDERER);
    g_gl_version  = (const char*)glGetString(GL_VERSION);
    g_glsl_ver    = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("OpenGL Vendor   : %s\n", g_gl_vendor   ? g_gl_vendor   : "(null)");
    printf("OpenGL Renderer : %s\n", g_gl_renderer ? g_gl_renderer : "(null)");
    printf("OpenGL Version  : %s\n", g_gl_version  ? g_gl_version  : "(null)");
    printf("GLSL Version    : %s\n", g_glsl_ver    ? g_glsl_ver    : "(null)");
    fflush(stdout);

    build_char_display_lists();
    update_window_title();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(g_w, g_h);
    glutCreateWindow("FOSDEM");

    init_gl();
    reshape(g_w, g_h);

    g_last_ms = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer_cb, 0);

    glutMainLoop();
    return 0;
}
