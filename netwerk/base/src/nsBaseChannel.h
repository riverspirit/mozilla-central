/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Google Inc.
 * Portions created by the Initial Developer are Copyright (C) 2005
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *  Darin Fisher <darin@meer.net>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef nsBaseChannel_h__
#define nsBaseChannel_h__

#include "nsString.h"
#include "nsAutoPtr.h"
#include "nsCOMPtr.h"
#include "nsHashPropertyBag.h"
#include "nsInputStreamPump.h"

#include "nsIChannel.h"
#include "nsIInputStream.h"
#include "nsIURI.h"
#include "nsILoadGroup.h"
#include "nsIStreamListener.h"
#include "nsIInterfaceRequestor.h"
#include "nsIProgressEventSink.h"
#include "nsITransport.h"
#include "nsThreadUtils.h"

//-----------------------------------------------------------------------------
// nsBaseChannel is designed to be subclassed.  The subclass is responsible for
// implementing the OpenContentStream method, which will be called by the
// nsIChannel::AsyncOpen and nsIChannel::Open implementations.
//
// nsBaseChannel implements nsIInterfaceRequestor to provide a convenient way
// for subclasses to query both the nsIChannel::notificationCallbacks and
// nsILoadGroup::notificationCallbacks for supported interfaces.
//
// nsBaseChannel implements nsITransportEventSink to support progress & status
// notifications generated by the transport layer.

class nsBaseChannel : public nsHashPropertyBag
                    , public nsIChannel
                    , public nsIInterfaceRequestor
                    , public nsITransportEventSink
                    , private nsIStreamListener
{
public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_NSIREQUEST
  NS_DECL_NSICHANNEL
  NS_DECL_NSIINTERFACEREQUESTOR
  NS_DECL_NSITRANSPORTEVENTSINK

  nsBaseChannel(); 

  // This method must be called to initialize the basechannel instance.
  nsresult Init() {
    return nsHashPropertyBag::Init();
  }

protected:
  // -----------------------------------------------
  // Methods to be implemented by the derived class:

  virtual ~nsBaseChannel() {}

private:
  // Implemented by subclass to supply data stream.  The parameter, async, is
  // true when called from nsIChannel::AsyncOpen and false otherwise.  When
  // async is true, the resulting stream will be used with a nsIInputStreamPump
  // instance.  This means that if it is a non-blocking stream that supports
  // nsIAsyncInputStream that it will be read entirely on the main application
  // thread, and its AsyncWait method will be called whenever ReadSegments
  // returns NS_BASE_STREAM_WOULD_BLOCK.  Otherwise, if the stream is blocking,
  // then it will be read on one of the background I/O threads, and it does not
  // need to implement ReadSegments.  If async is false, this method may return
  // NS_ERROR_NOT_IMPLEMENTED to cause the basechannel to implement Open in
  // terms of AsyncOpen (see NS_ImplementChannelOpen).
  // A callee is allowed to return an nsIChannel instead of an nsIInputStream.
  // That case will be treated as a redirect to the new channel.  By default
  // *channel will be set to null by the caller, so callees who don't want to
  // return one an just not touch it.
  virtual nsresult OpenContentStream(PRBool async, nsIInputStream **stream,
                                     nsIChannel** channel) = 0;

  // The basechannel calls this method from its OnTransportStatus method to
  // determine whether to call nsIProgressEventSink::OnStatus in addition to
  // nsIProgressEventSink::OnProgress.  This method may be overriden by the
  // subclass to enable nsIProgressEventSink::OnStatus events.  If this method
  // returns true, then the statusArg out param specifies the "statusArg" value
  // to pass to the OnStatus method.  By default, OnStatus messages are
  // suppressed.  The status parameter passed to this method is the status value
  // from the OnTransportStatus method.
  virtual PRBool GetStatusArg(nsresult status, nsString &statusArg) {
    return PR_FALSE;
  }

  // Called when the callbacks available to this channel may have changed.
  virtual void OnCallbacksChanged() {
  }

public:
  // ----------------------------------------------
  // Methods provided for use by the derived class:

  // Redirect to another channel.  This method takes care of notifying
  // observers of this redirect as well as of opening the new channel, if asked
  // to do so.  It also cancels |this| with the status code
  // NS_BINDING_REDIRECTED.  A failure return from this method means that the
  // redirect could not be performed (no channel was opened; this channel
  // wasn't canceled.)  The redirectFlags parameter consists of the flag values
  // defined on nsIChannelEventSink.
  nsresult Redirect(nsIChannel *newChannel, PRUint32 redirectFlags,
                    PRBool openNewChannel);

  // Tests whether a type hint was set. Subclasses can use this to decide
  // whether to call SetContentType.
  // NOTE: This is only reliable if the subclass didn't itself call
  // SetContentType, and should also not be called after OpenContentStream.
  PRBool HasContentTypeHint() const;

  // The URI member should be initialized before the channel is used, and then
  // it should never be changed again until the channel is destroyed.
  nsIURI *URI() {
    return mURI;
  }
  void SetURI(nsIURI *uri) {
    NS_ASSERTION(uri, "must specify a non-null URI");
    NS_ASSERTION(!mURI, "must not modify URI");
    NS_ASSERTION(!mOriginalURI, "how did that get set so early?");
    mURI = uri;
    mOriginalURI = uri;
  }
  nsIURI *OriginalURI() {
    return mOriginalURI;
  }

  // The security info is a property of the transport-layer, which should be
  // assigned by the subclass.
  nsISupports *SecurityInfo() {
    return mSecurityInfo; 
  }
  void SetSecurityInfo(nsISupports *info) {
    mSecurityInfo = info;
  }

  // Test the load flags
  PRBool HasLoadFlag(PRUint32 flag) {
    return (mLoadFlags & flag) != 0;
  }

  // This is a short-cut to calling nsIRequest::IsPending()
  PRBool IsPending() const {
    return (mPump != nsnull);
  }

  // Set the content length that should be reported for this channel.  Pass -1
  // to indicate an unspecified content length.
  void SetContentLength64(PRInt64 len);
  PRInt64 ContentLength64();

  // Helper function for querying the channel's notification callbacks.
  template <class T> void GetCallback(nsCOMPtr<T> &result) {
    GetInterface(NS_GET_TEMPLATE_IID(T), getter_AddRefs(result));
  }

  // Helper function for calling QueryInterface on this.
  nsQueryInterface do_QueryInterface() {
    return nsQueryInterface(static_cast<nsIChannel *>(this));
  }
  // MSVC needs this:
  nsQueryInterface do_QueryInterface(nsISupports *obj) {
    return nsQueryInterface(obj);
  }

  // If a subclass does not want to feed transport-layer progress events to the
  // base channel via nsITransportEventSink, then it may set this flag to cause
  // the base channel to synthesize progress events when it receives data from
  // the content stream.  By default, progress events are not synthesized.
  void EnableSynthesizedProgressEvents(PRBool enable) {
    mSynthProgressEvents = enable;
  }

  // Some subclasses may wish to manually insert a stream listener between this
  // and the channel's listener.  The following methods make that possible.
  void SetStreamListener(nsIStreamListener *listener) {
    mListener = listener;
  }
  nsIStreamListener *StreamListener() {
    return mListener;
  }

  // Pushes a new stream converter in front of the channel's stream listener.
  // The fromType and toType values are passed to nsIStreamConverterService's
  // AsyncConvertData method.  If invalidatesContentLength is true, then the
  // channel's content-length property will be assigned a value of -1.  This is
  // necessary when the converter changes the length of the resulting data
  // stream, which is almost always the case for a "stream converter" ;-)
  // This function optionally returns a reference to the new converter.
  nsresult PushStreamConverter(const char *fromType, const char *toType,
                               PRBool invalidatesContentLength = PR_TRUE,
                               nsIStreamListener **converter = nsnull);

private:
  NS_DECL_NSISTREAMLISTENER
  NS_DECL_NSIREQUESTOBSERVER

  // Called to setup mPump and call AsyncRead on it.
  nsresult BeginPumpingData();

  // Called when the callbacks available to this channel may have changed.
  void CallbacksChanged() {
    mProgressSink = nsnull;
    mQueriedProgressSink = PR_FALSE;
    OnCallbacksChanged();
  }

  // Handle an async redirect callback.  This will only be called if we
  // returned success from AsyncOpen while posting a redirect runnable.
  void HandleAsyncRedirect(nsIChannel* newChannel);

  class RedirectRunnable : public nsRunnable
  {
  public:
    RedirectRunnable(nsBaseChannel* chan, nsIChannel* newChannel)
      : mChannel(chan), mNewChannel(newChannel)
    {
      NS_PRECONDITION(newChannel, "Must have channel to redirect to");
    }
    
    NS_IMETHOD Run()
    {
      mChannel->HandleAsyncRedirect(mNewChannel);
      return NS_OK;
    }

  private:
    nsRefPtr<nsBaseChannel> mChannel;
    nsCOMPtr<nsIChannel> mNewChannel;
  };
  friend class RedirectRunnable;

  nsRefPtr<nsInputStreamPump>         mPump;
  nsCOMPtr<nsIInterfaceRequestor>     mCallbacks;
  nsCOMPtr<nsIProgressEventSink>      mProgressSink;
  nsCOMPtr<nsIURI>                    mOriginalURI;
  nsCOMPtr<nsIURI>                    mURI;
  nsCOMPtr<nsILoadGroup>              mLoadGroup;
  nsCOMPtr<nsISupports>               mOwner;
  nsCOMPtr<nsISupports>               mSecurityInfo;
  nsCOMPtr<nsIStreamListener>         mListener;
  nsCOMPtr<nsISupports>               mListenerContext;
  nsCString                           mContentType;
  nsCString                           mContentCharset;
  PRUint32                            mLoadFlags;
  nsresult                            mStatus;
  PRPackedBool                        mQueriedProgressSink;
  PRPackedBool                        mSynthProgressEvents;
  PRPackedBool                        mWasOpened;
};

#endif // !nsBaseChannel_h__
