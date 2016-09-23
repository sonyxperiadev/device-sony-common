/*
 * Copyright (C) ST-Ericsson SA 2010
 * Copyright 2010, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors: johan.xj.palmaeus@stericsson.com
 *          stuart.macdonald@stericsson.com
 *          for ST-Ericsson
 */

/*
 * Native part of the generic RX FmRadio inteface
 */

#define ALOG_TAG "FmReceiverServiceNative"

// #define LOG_NDEBUG 1

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>


#include "jni.h"
#include "JNIHelp.h"
#include "android_fmradio_Receiver.h"
#include <utils/Log.h>


/* *INDENT-OFF* */
namespace android {


// state machine

static const ValidEventsForStates_t IsValidRxEventForState = {
  /* this table defines valid transitions. (turn off indent, we want this easy readable) */
             /* FMRADIO_STATE_ IDLE,STARTING,STARTED,PAUSED,SCANNING,EXTRA_COMMAND */

   /* FMRADIO_EVENT_START */         {true ,false,false,false,false,false},
   /* FMRADIO_EVENT_START_ASYNC */   {true ,false,false,false,false,false},
   /* FMRADIO_EVENT_PAUSE */         {false,false,true, true, false,false},
   /* FMRADIO_EVENT_RESUME */        {false,false,true, true, false,false},
   /* FMRADIO_EVENT_RESET */         {true, true, true, true, true, true },
   /* FMRADIO_EVENT_GET_FREQUENCY */ {false,false,true, true, false,false},
   /* FMRADIO_EVENT_SET_FREQUENCY */ {false,false,true, true, false,false},
   /* FMRADIO_EVENT_SET_PARAMETER */ {false,false,true, true, true, true },
   /* FMRADIO_EVENT_STOP_SCAN */     {true, true, true, true, true, true },
   /* FMRADIO_EVENT_EXTRA_COMMAND */ {true, true, true, true, true, true },
   /* Rx Only */
   /* FMRADIO_EVENT_GET_PARAMETER */ {false,false,true, true, true, true },
   /* FMRADIO_EVENT_GET_SIGNAL_STRENGTH */{false,false,true,true,false,false},
   /* FMRADIO_EVENT_SCAN */          {false,false,true, true, false,false},
   /* FMRADIO_EVENT_FULL_SCAN */     {false,false,true, true, false,false},
   // Tx Only - never allowed
   /* FMRADIO_EVENT_BLOCK_SCAN */    {false,false,false,false,false,false},
};
/*  *INDENT-ON*  */

/* Callbacks to java layer */

static void androidFmRadioRxCallbackOnStateChanged(int oldState,
                                                   int newState);
static void androidFmRadioRxCallbackOnError(void);

static void androidFmRadioRxCallbackOnStarted(void);

static void androidFmRadioRxCallbackOnScan(int foundFreq,
                                           int signalStrength,
                                           int scanDirection,
                                           bool aborted);
static void androidFmRadioRxCallbackOnFullScan(int noItems,
                                               int *frequencies,
                                               int *sigStrengths,
                                               bool aborted);
static void androidFmRadioRxCallbackOnForcedReset(enum fmradio_reset_reason_t reason);

static void androidFmRadioRxCallbackOnVendorForcedReset(enum fmradio_reset_reason_t reason);

static void androidFmRadioRxCallbackOnSignalStrengthChanged(int newLevel);

static void androidFmRadioRxCallbackOnRDSDataFound(struct
                                                   fmradio_rds_bundle_t
                                                   *t, int frequency);

static void androidFmRadioRxCallbackOnPlayingInStereo(int
                                                      isPlayingInStereo);

static void androidFmRadioRxCallbackOnExtraCommand(char* command,
                                                   struct
                                                   fmradio_extra_command_ret_item_t
                                                   *retItem);

struct FmSession_t fmTransmitterSession;

struct FmSession_t fmReceiverSession = {
    NULL,
    NULL,
    false,
    FMRADIO_STATE_IDLE,
    NULL,
    &IsValidRxEventForState,
    NULL,
    NULL,
    &fmTransmitterSession,
    NULL,
    FMRADIO_STATE_IDLE,
    false,
    false,
    false,
    &rx_tx_common_mutex,
    PTHREAD_COND_INITIALIZER,
    NULL,
};

/*
 *  function calls from java layer.
 */

static jint androidFmRadioRxGetState(JNIEnv * env, jobject obj)
{
    FmRadioState_t state;

    ALOGI("androidFmRadioRxGetState, state\n");

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);
    state = fmReceiverSession.state;
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);
    return state;
}

/* common ones with tx, just forward to the generic androidFmRadioxxxxx version */

static bool
androidFmRadioRxStart(JNIEnv * env, jobject obj, int lowFreq,
                      int highFreq, int defaultFreq, int grid)
{
    ALOGI("androidFmRadioRxStart. LowFreq %d, HighFreq %d, DefaultFreq %d, grid %d.", lowFreq, highFreq, defaultFreq, grid);

    if (fmReceiverSession.jobj == NULL)
        fmReceiverSession.jobj = env->NewGlobalRef(obj);
    return androidFmRadioStart(&fmReceiverSession, FMRADIO_RX, false, lowFreq,
                               highFreq, defaultFreq, grid);
}


static bool
androidFmRadioRxStartAsync(JNIEnv * env, jobject obj, int lowFreq,
                           int highFreq, int defaultFreq, int grid)
{
  //  ALOGI("androidFmRadioRxStartAsync...");

    if (fmReceiverSession.jobj == NULL)
        fmReceiverSession.jobj = env->NewGlobalRef(obj);
    return androidFmRadioStart(&fmReceiverSession, FMRADIO_RX,true,
                               lowFreq, highFreq, defaultFreq, grid);
}

static void androidFmRadioRxPause(JNIEnv * env, jobject obj)
{
  //  ALOGI("androidFmRadioRxPause\n");

    (void)androidFmRadioPause(&fmReceiverSession);
}

static void androidFmRadioRxResume(JNIEnv * env, jobject obj)
{
  //  ALOGI("androidFmRadioRxResume\n");
    (void)androidFmRadioResume(&fmReceiverSession);
}

static jboolean androidFmRadioRxReset(JNIEnv * env, jobject obj)
{
    int retval = 0;

  //  ALOGI("androidFmRadioRxReset");
    retval = androidFmRadioReset(&fmReceiverSession);

    if (retval >= 0 && fmReceiverSession.state == FMRADIO_STATE_IDLE &&
        fmReceiverSession.jobj != NULL) {
        env->DeleteGlobalRef(fmReceiverSession.jobj);
        fmReceiverSession.jobj = NULL;
    }

    return retval;
}

static int androidFmRadioRxMute(JNIEnv * env, jobject obj, jint mute)
{
  //  ALOGI("androidFmRadioRxPause\n");

    return androidFmRadioMute(&fmReceiverSession, mute);
}

static int
androidFmRadioRxSetFrequency(JNIEnv * env, jobject obj, jint frequency)
{
  //  ALOGI("androidFmRadioRxSetFrequency tuneTo:%d\n", (int) frequency);
    return androidFmRadioSetFrequency(&fmReceiverSession, (int) frequency);
}

static jint androidFmRadioRxGetFrequency(JNIEnv * env, jobject obj)
{
  //  ALOGI("androidFmRadioRxGetFrequency:\n");
    return androidFmRadioGetFrequency(&fmReceiverSession);
}

static jint androidFmRadioRxStopScan(JNIEnv * env, jobject obj)
{
  //  ALOGI("androidFmRadioRxStopScan\n");
    return androidFmRadioStopScan(&fmReceiverSession);
}

/* the rest of the calls are specific for RX */

static jint androidFmRadioRxGetSignalStrength(JNIEnv * env, jobject obj)
{
    int retval = SIGNAL_STRENGTH_UNKNOWN;

  //  ALOGI("androidFmRadioRxGetSignalStrength\n");

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    if (!androidFmRadioIsValidEventForState
        (&fmReceiverSession, FMRADIO_EVENT_GET_SIGNAL_STRENGTH)) {
        goto drop_lock;
    }

    if (fmReceiverSession.vendorMethods_p->get_signal_strength) {
        /* if in pause state temporary resume */
        androidFmRadioTempResumeIfPaused(&fmReceiverSession);

        retval =
            fmReceiverSession.vendorMethods_p->
            get_signal_strength(&fmReceiverSession.vendorData_p);

        if (retval < 0) {
            retval = SIGNAL_STRENGTH_UNKNOWN;
        } else if (retval > SIGNAL_STRENGTH_MAX) {
            retval = SIGNAL_STRENGTH_MAX;
        }
        androidFmRadioPauseIfTempResumed(&fmReceiverSession);
    }

  drop_lock:

    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    return retval;
}

static jboolean
androidFmRadioRxIsPlayingInStereo(JNIEnv * env, jobject obj)
{
    bool retval;

  //  ALOGI("androidFmRadioRxIsPlayingInStereo:\n");

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    /* if we haven't register we don't know yet */
    if (!fmReceiverSession.isRegistered) {
        retval = false;
        goto drop_lock;
    }
    // valid in all states
    if (fmReceiverSession.vendorMethods_p->is_playing_in_stereo != NULL) {
        retval =
            fmReceiverSession.vendorMethods_p->
            is_playing_in_stereo(&fmReceiverSession.vendorData_p);
    } else {
        retval = false;
    }

  drop_lock:

    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    return retval;
}

static int
androidFmRadioRxIsRDSDataSupported(JNIEnv * env, jobject obj)
{
    bool retval;

 //   ALOGI("androidFmRadioRxIsRDSDataSupported:\n");

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    /* if we haven't register we don't know yet */
    if (!fmReceiverSession.isRegistered) {
        retval = false;
        goto drop_lock;
    }
    // valid in all states
    if (fmReceiverSession.vendorMethods_p->is_rds_data_supported != NULL) {
        retval =
            fmReceiverSession.vendorMethods_p->
            is_rds_data_supported(&fmReceiverSession.vendorData_p);
    } else {
        retval = false;
    }

  drop_lock:

    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);
    return retval;
}

static jboolean
androidFmRadioRxIsTunedToValidChannel(JNIEnv * env, jobject obj)
{
    bool retval;

 //   ALOGI("androidFmRadioRxIsTunedToValidChannel:\n");

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    /* if we haven't register we don't know yet */
    if (!fmReceiverSession.isRegistered) {
        retval = false;
        goto drop_lock;
    }
    // valid in all states
    if (fmReceiverSession.vendorMethods_p->is_tuned_to_valid_channel != NULL) {
        retval =
            fmReceiverSession.vendorMethods_p->
            is_tuned_to_valid_channel(&fmReceiverSession.vendorData_p);
    } else {
        retval = false;
    }

  drop_lock:

    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);
    return retval;
}

static bool androidFmRadioRxScan(enum fmradio_seek_direction_t scanDirection, jint *frequency)
{
    int signalStrength = -1;
    int retval;

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);
    // we should still be in SCANNING mode, but we can't be 100.00 % sure since main thread released lock
    // before we could run

    if (fmReceiverSession.state != FMRADIO_STATE_SCANNING) {
        ALOGE("execute_androidFmRadioRxScan - warning, state not scanning");
    }

    /*
     * if mode has been changed to IDLE in the mean time by main thread,
     * exit the worker thread gracefully
     */
    if (fmReceiverSession.state == FMRADIO_STATE_IDLE) {
        goto drop_lock;
    }

    if (pthread_cond_signal(&fmReceiverSession.sync_cond) != 0) {
        ALOGE("execute_androidFmRadioRxScan - warning, signal failed");
    }
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    retval =
        fmReceiverSession.vendorMethods_p->scan(&fmReceiverSession.
                                                vendorData_p,
                                                scanDirection);

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    if (retval >= 0) {
        // also get signal strength (if supported)
        if (fmReceiverSession.vendorMethods_p->get_signal_strength)
            signalStrength =
                fmReceiverSession.vendorMethods_p->
                get_signal_strength(&fmReceiverSession.vendorData_p);
    }
    /*
     * if state has changed we should keep it, probably a forced reset
     */
    if (fmReceiverSession.state != FMRADIO_STATE_SCANNING) {
        ALOGI("State changed while scanning (state now %d), keeping",
             fmReceiverSession.state);
        retval = -1;
    } else {
        FMRADIO_SET_STATE(&fmReceiverSession, FMRADIO_STATE_STARTED);
        // if we failed but we have a pending abort just read the current frequency to give a proper
        // onScan return

        if (retval < 0 && fmReceiverSession.lastScanAborted &&
            fmReceiverSession.vendorMethods_p->get_frequency) {
            retval = fmReceiverSession.vendorMethods_p->get_frequency(&fmReceiverSession.vendorData_p);
        }
    }

    fmReceiverSession.pendingPause = false;

    if (retval >= 0) {
        *frequency = retval;
    }

drop_lock:
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    return 0;
}

static bool
androidFmRadioRxScanUp(JNIEnv * env, jobject obj, jint *frequency)
{
  //  ALOGI("androidFmRadioRxScanUp\n");
    FMRADIO_SET_STATE(&fmReceiverSession, FMRADIO_STATE_SCANNING);
    androidFmRadioRxScan(FMRADIO_SEEK_UP, frequency);
    return 0;
}

static bool
androidFmRadioRxScanDown(JNIEnv * env, jobject obj, jint *frequency)
{
  //  ALOGI("androidFmRadioRxScanDown\n");
    FMRADIO_SET_STATE(&fmReceiverSession, FMRADIO_STATE_SCANNING);
    return androidFmRadioRxScan(FMRADIO_SEEK_DOWN, frequency);
}

static int androidFmRadioRxFullScan(int *frequencies)
{
    int retval;
    int *frequencies_p = NULL;
    int *rssi_p = NULL;

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    // we should still be in SCANNING mode, but we can't be 100.00 % sure since main thread released lock
    // before we could run

    if (fmReceiverSession.state != FMRADIO_STATE_SCANNING) {
        ALOGE("execute_androidFmRadioRxFullScan - warning, state not scanning\n");
    }

    /*
     * if mode has been changed to IDLE in the mean time by main thread,
     * exit the worker thread gracefully
     */
    if (fmReceiverSession.state == FMRADIO_STATE_IDLE) {
        goto drop_lock;
    }

    if (pthread_cond_signal(&fmReceiverSession.sync_cond) != 0) {
        ALOGE("execute_androidFmRadioRxFullScan - warning, signal failed\n");
    }
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    retval = fmReceiverSession.vendorMethods_p->full_scan(&fmReceiverSession.
                                                    vendorData_p,
                                                    &frequencies_p,
                                                    &rssi_p);
    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    /*
     * if state has changed we should keep it, probably a forced pause or
     * forced reset
     */
    if (fmReceiverSession.state != FMRADIO_STATE_SCANNING) {
        ALOGI("State changed while scanning (state now %d), keeping\n",
             fmReceiverSession.state);
        retval = -1;
    } else {
        FMRADIO_SET_STATE(&fmReceiverSession, FMRADIO_STATE_STARTED);
    }

    if (retval >= 0) {
        for (int i=0; i < 50; i++) {
           if (frequencies_p[i] <= 0)
                break;
           frequencies[i] = frequencies_p[i];
        }
    }

    if (frequencies_p != NULL) {
        free(frequencies_p);
    }

    if (rssi_p != NULL) {
        free(rssi_p);
    }

    drop_lock:
    /* Wake up the main thread if it is currently waiting on the condition variable */
    if (pthread_cond_signal(&fmReceiverSession.sync_cond) != 0) {
        ALOGE("execute_androidFmRadioRxFullScan - signal failed\n");
    }
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    return 0;
}

static int androidFmRadioRxStartFullScan(JNIEnv * env, jobject obj, int *frequencies)
{
  //  ALOGI("androidFmRadioRxStartFullScan\n");
    int retval = 0;

    FMRADIO_SET_STATE(&fmReceiverSession, FMRADIO_STATE_SCANNING);
    androidFmRadioRxFullScan(frequencies);
    return retval;
}

static void androidFmRadioRxSetForceMono(JNIEnv * env, jobject obj,
                                         jboolean forceMono)
{
    int retval = -1;

   // ALOGI("androidFmRadioRxSetForceMono\n");

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    if (!androidFmRadioIsValidEventForState
        (&fmReceiverSession, FMRADIO_EVENT_SET_PARAMETER)) {
        retval = FMRADIO_INVALID_STATE;
        goto drop_lock;
    }


    if (fmReceiverSession.vendorMethods_p->set_force_mono) {
        /* if in pause state temporary resume */
        androidFmRadioTempResumeIfPaused(&fmReceiverSession);

        retval =
            fmReceiverSession.vendorMethods_p->
            set_force_mono(&fmReceiverSession.vendorData_p, forceMono);

        androidFmRadioPauseIfTempResumed(&fmReceiverSession);
    } else {
        retval = FMRADIO_UNSUPPORTED_OPERATION;
    }

  drop_lock:
    if (retval == FMRADIO_INVALID_STATE) {
        THROW_INVALID_STATE(&fmReceiverSession);
    } else if (retval < 0) {
        THROW_IO_ERROR(&fmReceiverSession);
    }

    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);
}

static void
androidFmRadioRxSetThreshold(JNIEnv * env, jobject obj, jint threshold)
{
    int retval;

  //  ALOGI("androidFmRadioRxSetThreshold threshold:%d\n", (int) threshold);

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);
    if (!androidFmRadioIsValidEventForState
        (&fmReceiverSession, FMRADIO_EVENT_SET_PARAMETER)) {
        retval = FMRADIO_INVALID_STATE;
        goto drop_lock;
    }


    if (fmReceiverSession.vendorMethods_p->set_threshold) {
        /* if in pause state temporary resume */
        androidFmRadioTempResumeIfPaused(&fmReceiverSession);

        retval =
            fmReceiverSession.
            vendorMethods_p->set_threshold(&fmReceiverSession.vendorData_p,
                                          threshold);
        /* if in pause state temporary resume */
        androidFmRadioPauseIfTempResumed(&fmReceiverSession);
    } else {
        retval = FMRADIO_UNSUPPORTED_OPERATION;
    }

    if (retval == FMRADIO_INVALID_STATE) {
        THROW_INVALID_STATE(&fmReceiverSession);
    } else if (retval < 0) {
        THROW_IO_ERROR(&fmReceiverSession);
    }

  drop_lock:
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);
}

static jint androidFmRadioRxGetThreshold(JNIEnv * env, jobject obj)
{
    int retval;

   // ALOGI("androidFmRadioRxGetThreshold\n");
    pthread_mutex_lock(fmReceiverSession.dataMutex_p);

    if (!androidFmRadioIsValidEventForState
        (&fmReceiverSession, FMRADIO_EVENT_GET_PARAMETER)) {
        retval = FMRADIO_INVALID_STATE;
        goto drop_lock;
    }

    if (fmReceiverSession.vendorMethods_p->get_threshold) {
        /* if in pause state temporary resume */
        androidFmRadioTempResumeIfPaused(&fmReceiverSession);
        retval =
            fmReceiverSession.
            vendorMethods_p->get_threshold(&fmReceiverSession.vendorData_p);
        androidFmRadioPauseIfTempResumed(&fmReceiverSession);
    } else {
        retval = FMRADIO_UNSUPPORTED_OPERATION;
    }
  drop_lock:

    if (retval == FMRADIO_INVALID_STATE) {
        THROW_INVALID_STATE(&fmReceiverSession);
    } else if (retval < 0) {
        THROW_IO_ERROR(&fmReceiverSession);
    }
    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);

    return retval;
}

static jboolean androidFmRadioRxSendExtraCommand(JNIEnv * env, jobject obj,
                                                 jstring command,
                                                 jobjectArray parameters)
{
   // ALOGI("androidFmRadioRxSendExtraCommand");

/* we need to set jobj since this might be called before start */

    if (fmReceiverSession.jobj == NULL)
        fmReceiverSession.jobj = env->NewGlobalRef(obj);

    androidFmRadioSendExtraCommand(&fmReceiverSession, env, command,
                                   parameters);

    return true;
}

jboolean tune(JNIEnv *env, jobject thiz, jfloat freq)
{
    int ret = 0;
    int tmp_freq;

    tmp_freq = (int)(freq * 1000);        //Eg, 87.5 * 10 --> 875
    ret = androidFmRadioRxSetFrequency(env, thiz, tmp_freq);

    ALOGD("%s, [ret=%d]\n", __func__, ret);
    return ret <= 0 ? JNI_FALSE:JNI_TRUE;
}

jboolean powerUp(JNIEnv *env, jobject thiz, jfloat freq)
{
    int ret = 0;
    int tmp_freq;

    if (androidFmRadioRxGetState(env, thiz) != FMRADIO_STATE_IDLE) {
       androidFmRadioRxResume(env, thiz);
       return false;
   }

 //   ALOGI("%s, [freq=%d]\n", __func__, (int)freq);
    tmp_freq = (int)(freq * 1000);        //Eg, 87.5 * 10 --> 875
    ret = androidFmRadioRxStart(env, thiz, 87500, 108000,  tmp_freq, 100);
 //   ALOGD("%s, [ret=%d]\n", __func__, ret);
    return ret?JNI_FALSE:JNI_TRUE;
}

jint setMute(JNIEnv *env, jobject thiz, jboolean mute)
{
    int ret = 1;

    ret = androidFmRadioRxMute(env, thiz, (int)mute);
    if (ret) {
        ALOGE("%s, error, [ret=%d]\n", __func__, ret);
    }
   // ALOGD("%s, [mute=%d] [ret=%d]\n", __func__, (int)mute, ret);
    return ret?JNI_FALSE:JNI_TRUE;
}

jint isRdsSupport(JNIEnv *env, jobject thiz)
{
    int ret = 0;
/*
    ret = androidFmRadioRxIsRDSDataSupported(env, thiz);
    if (!ret) {
        ALOGE("%s, error, [ret=%d]\n", __func__, ret);
    }
    ALOGD("%s, [ret=%d]\n", __func__, ret);
*/
    return JNI_TRUE;//ret?JNI_TRUE:JNI_FALSE;
}

jshort readRds(JNIEnv *env, jobject thiz)
{
    int ret = 0;
   // ALOGD("%s, [ret=%d]\n", __func__, ret);
    return 0x0040; //Java: RDS_EVENT_LAST_RADIOTEXT
}

jint setRds(JNIEnv *env, jobject thiz, jboolean rdson)
{
    int ret = 0;
    int onoff = -1;
/*
    ret = androidFmRadioRxSetRDS(env, thiz, rdson);
    if (ret) {
        ALOGE("%s, error, [ret=%d]\n", __func__, ret);
    }
    ALOGD("%s, [onoff=%d] [ret=%d]\n", __func__, onoff, ret);
*/
    return JNI_TRUE;
}


jbyteArray getLrText(JNIEnv *env, jobject thiz)
{
    int ret = 0;
    int len = 0;
    jbyteArray LastRadioText;
    struct fmradio_rds_bundle_t fmradio_rds_bundle;
    // ALOGD("%s, enter\n", __func__, ret);

    ret = fmReceiverSession.vendorMethods_p->get_rds(&fmReceiverSession.vendorData_p, &fmradio_rds_bundle);//FMR_get_ps(g_idx, &ps, &ps_len);

    if (ret) {
       // ALOGE("%s, error, [ret=%d]\n", __func__, ret);
        return NULL;
    }

    len = strlen(fmradio_rds_bundle.psn);
    LastRadioText = env->NewByteArray(len);
    env->SetByteArrayRegion(LastRadioText, 0,  len, (const jbyte*)fmradio_rds_bundle.psn);


  //  ALOGD("%s, exit: [ret=%d]\n", __func__, ret);
    return LastRadioText;
}

jbyteArray getPs(JNIEnv *env, jobject thiz)
{
    int ret = 0;
    jbyteArray PSName;
    uint8_t *rt = NULL;
    int rt_len = 0;
 //   ALOGD("%s, enter\n", __func__, ret);
    ret = 1;//FMR_get_rt(g_idx, &rt, &rt_len);
    if (ret) {
        ALOGE("%s, error, [ret=%d]\n", __func__, ret);
        return NULL;
    }
    PSName = env->NewByteArray(rt_len);
    env->SetByteArrayRegion(PSName, 0, rt_len, (const jbyte*)rt);
 //   ALOGD("%s, [ret=%d]\n", __func__, ret);
    return PSName;
}


jshortArray autoScan(JNIEnv *env, jobject thiz)
{
    int ret = 0;
    jshortArray scanChlarray;
    int chl_cnt = 0;
    int ScanTBL[50];
    int short fixedTable[50];

    ret = androidFmRadioRxStartFullScan(env, thiz, ScanTBL);
    if (ret < 0) {
        ALOGE("scan failed!\n");
        scanChlarray = NULL;
        goto out;
    }

    for (int i =0; i <= 50; i++) {
         int val = ScanTBL[i]/100;
         if (val <= 0)
              break;
         fixedTable[i] = val & 0xFFFF;
       //  ALOGD("Add freq: %d", fixedTable[i]);
         chl_cnt++;
    }

    if (chl_cnt > 0) {
        scanChlarray = env->NewShortArray(chl_cnt);
        env->SetShortArrayRegion(scanChlarray, 0, chl_cnt, fixedTable);
    } else {
        ALOGE("cnt error, [cnt=%d]\n", chl_cnt);
        scanChlarray = NULL;
    }

out:
    ALOGD("%s, [cnt=%d] [ret=%d]\n", __func__, chl_cnt, ret);
    return scanChlarray;
}

jint switchAntenna(JNIEnv *env, jobject thiz, jint antenna)
{
    int ret = 0;
    int ana = -1;
    ALOGD("%s: [antenna=%d] [ret=%d]\n", __func__, ana, ret);
    return ret;
}

jboolean stopScan(JNIEnv *env, jobject thiz)
{
    int ret = 0;

    ret = androidFmRadioRxStopScan(env, thiz);
    if (ret) {
        ALOGE("%s, error, [ret=%d]\n", __func__, ret);
    }
    ALOGD("%s, [ret=%d]\n", __func__, ret);
    return ret?JNI_TRUE:JNI_FALSE;
}


jfloat seek(JNIEnv *env, jobject thiz, jfloat freq, jboolean isUp) //jboolean isUp;
{
    int ret = 0;
    int tmp_freq;
    jint ret_freq;
    float val;

    tmp_freq = (int)(freq * 1000);       //Eg, 87.55 * 100 --> 8755

    ret = setMute(env, thiz, 1);
    if (ret) {
        ALOGE("%s, error, [ret=%d]\n", __func__, ret);
    }
  //  ALOGD("%s, [mute] [ret=%d]\n", __func__, ret);

    if (isUp) {
          ret = androidFmRadioRxScanUp(env, thiz, &ret_freq);
    } else {
          ret = androidFmRadioRxScanDown(env, thiz, &ret_freq);
    }

    if (ret) {
        ret_freq = tmp_freq; //seek error, so use original freq
    }

    val = roundf((ret_freq/1000.00F) * 100) / 100;

  //  ALOGD("%s, [freq=%f] [ret=%d]\n", __func__, val, ret);

    return val;
}

jboolean powerDown(JNIEnv *env, jobject thiz, jint type)
{
    int ret = 0;

    ret = androidFmRadioRxReset(env, thiz);

    ALOGD("%s, [ret=%d]\n", __func__, ret);
    return ret?JNI_TRUE:JNI_FALSE;
}

static jboolean openDev(JNIEnv * env, jobject obj,
                                   jboolean receiveRDS) {
    return JNI_TRUE;
}

static jboolean closeDev(JNIEnv * env, jobject obj,
                                   jboolean receiveRDS) {
    return JNI_TRUE;
}


static JNINativeMethod gMethods[] = {
    {"openDev", "()Z", (void*)openDev },
    {"closeDev", "()Z", (void*)closeDev },
    {"powerUp", "(F)Z", (void*)powerUp },
    {"powerDown", "(I)Z", (void*)powerDown },
    {"tune", "(F)Z", (void*)tune },
    {"seek", "(FZ)F", (void*)seek },
    {"autoScan",  "()[S", (void*)autoScan },
    {"stopScan",  "()Z", (void*)stopScan },
    {"setRds",    "(Z)I", (void*)setRds  },
    {"readRds",   "()S", (void*)readRds },
    {"getPs",     "()[B", (void*)getPs  },
    {"getLrText", "()[B", (void*)getLrText},
    {"setMute",	"(Z)I", (void*)setMute},
    {"isRdsSupport",	"()I", (void*)isRdsSupport},
    {"switchAntenna", "(I)I", (void*)switchAntenna},
};

int registerAndroidFmRadioReceiver(JavaVM * vm, JNIEnv * env)
{
    ALOGI("registerAndroidFmRadioReceiver\n");
    jclass clazz;

    pthread_mutex_lock(fmReceiverSession.dataMutex_p);
    fmReceiverSession.jvm_p = vm;

    struct bundle_descriptor_offsets_t *bundle_p =
        (struct bundle_descriptor_offsets_t *)
        malloc(sizeof(struct bundle_descriptor_offsets_t));

    clazz = env->FindClass("android/os/Bundle");
    bundle_p->mClass = (jclass) env->NewGlobalRef(clazz);
    bundle_p->mConstructor = env->GetMethodID(clazz, "<init>", "()V");
    bundle_p->mPutInt =
        env->GetMethodID(clazz, "putInt", "(Ljava/lang/String;I)V");
    bundle_p->mPutShort =
        env->GetMethodID(clazz, "putShort", "(Ljava/lang/String;S)V");
    bundle_p->mPutIntArray =
        env->GetMethodID(clazz, "putIntArray", "(Ljava/lang/String;[I)V");
    bundle_p->mPutShortArray =
        env->GetMethodID(clazz, "putShortArray",
                         "(Ljava/lang/String;[S)V");
    bundle_p->mPutString =
        env->GetMethodID(clazz, "putString",
                         "(Ljava/lang/String;Ljava/lang/String;)V");

    fmReceiverSession.bundleOffsets_p = bundle_p;

    pthread_mutex_unlock(fmReceiverSession.dataMutex_p);
    return jniRegisterNativeMethods(env,
                                    "com/android/fmradio/FmNative",
                                    gMethods, NELEM(gMethods));
}

/* *INDENT-OFF* */
};                              // namespace android
