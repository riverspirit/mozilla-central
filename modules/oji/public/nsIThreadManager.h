/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#ifndef nsIThreadManager_h___
#define nsIThreadManager_h___

#include "nsISupports.h"

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Thread Manager
// This interface provides thread primitives.

class nsIRunnable;

class nsIThreadManager : public nsISupports {
public:
	/**
	 * Returns a unique identifier for the "current" system thread.
	 */
	NS_IMETHOD
	GetCurrentThread(PRUint32* threadID) = 0;

	/**
	 * Pauses the current thread for the specified number of milliseconds.
	 * If milli is zero, then merely yields the CPU if another thread of
	 * greater or equal priority.
	 */
	NS_IMETHOD
	Sleep(PRUint32 milli = 0) = 0;
	
	/**
	 * Creates a unique monitor for the specified address, and makes the
	 * current system thread the owner of the monitor.
	 */
	NS_IMETHOD
	EnterMonitor(void* address) = 0;
	
	/**
	 * Exits the monitor associated with the address.
	 */
	NS_IMETHOD
	ExitMonitor(void* address) = 0;
	
	/**
	 * Waits on the monitor associated with the address (must be entered already).
	 * If milli is 0, wait indefinitely.
	 */
	NS_IMETHOD
	Wait(void* address, PRUint32 milli = 0) = 0;

	/**
	 * Notifies a single thread waiting on the monitor associated with the address (must be entered already).
	 */
	NS_IMETHOD
	Notify(void* address) = 0;

	/**
	 * Notifies all threads waiting on the monitor associated with the address (must be entered already).
	 */
	NS_IMETHOD
	NotifyAll(void* address) = 0;

	/**
	 * Thread creation primitives.
	 */
	NS_IMETHOD
	CreateThread(PRUint32* threadID, nsIRunnable* runnable) = 0;
};

#define NS_ITHREADMANAGER_IID                           \
{ /* 97bb54c0-6846-11d2-801f-00805f71101c */            \
	0x97bb54c0,											\
	0x6846,												\
	0x11d2,												\
	{0x80, 0x1f, 0x00, 0x80, 0x5f, 0x71, 0x10, 0x1c}	\
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Runnable
// This interface represents the invocation of a new thread.

class nsIRunnable : public nsISupports {
public:
	/**
	 * Defines an entry point for a newly created thread.
	 */
	NS_IMETHOD
	Run() = 0;
};

#define NS_IRUNNABLE_IID								\
{ /* 930f3d70-6849-11d2-801f-00805f71101c */			\
	0x930f3d70,											\
	0x6849,												\
	0x11d2,												\
	{0x80, 0x1f, 0x00, 0x80, 0x5f, 0x71, 0x10, 0x1c}	\
}

#endif /* nsIThreadManager_h___ */
