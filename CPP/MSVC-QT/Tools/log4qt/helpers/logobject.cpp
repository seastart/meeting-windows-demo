﻿/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logobject.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * Copyright 2007 Martin Heinrich
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
 ******************************************************************************/


 /******************************************************************************
  * Dependencies
  ******************************************************************************/


#include "log4qt/helpers/logobject.h"

#include <QtCore/QDebug>



namespace Log4Qt
{


	/**************************************************************************
	 * Declarations
	 **************************************************************************/



	 /**************************************************************************
	  * C helper functions
	  **************************************************************************/



	  /**************************************************************************
	   * Class implementation: LogObject
	   **************************************************************************/



	   /**************************************************************************
		* Implementation: Operators, Helper
		**************************************************************************/


#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug debug,
		const LogObject& rLogObject)
	{
		return rLogObject.debug(debug);
	}
#endif // QT_NO_DEBUG_STREAM



} // namespace Log4Qt
