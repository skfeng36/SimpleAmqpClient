/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * ``The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is SimpleAmqpClient for RabbitMQ.
 *
 * The Initial Developer of the Original Code is Alan Antonuk.
 * Original code is Copyright (C) Alan Antonuk.
 *
 * All Rights Reserved.
 *
 * Contributor(s): ______________________________________.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU General Public License Version 2 or later (the "GPL"), in
 * which case the provisions of the GPL are applicable instead of those
 * above. If you wish to allow use of your version of this file only
 * under the terms of the GPL, and not to allow others to use your
 * version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the
 * notice and other provisions required by the GPL. If you do not
 * delete the provisions above, a recipient may use your version of
 * this file under the terms of any one of the MPL or the GPL.
 *
 * ***** END LICENSE BLOCK *****
 */

#include "SimplePublisher.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <sstream>

namespace AmqpClient
{

SimplePublisher::SimplePublisher(Channel::ptr_t channel, const std::string& publisher_name) :
	m_channel(channel), m_publisherExchange(publisher_name)
{
	if (m_publisherExchange == "")
	{
		m_publisherExchange = "SimplePublisher_";
		boost::uuids::random_generator uuid_gen;
		boost::uuids::uuid guid(uuid_gen());
		m_publisherExchange += boost::lexical_cast<std::string>(guid);
	}


	m_channel->DeclareExchange(m_publisherExchange, "fanout", false, false, false);
}

SimplePublisher::~SimplePublisher()
{
	m_channel->DeleteExchange(m_publisherExchange);
}

void SimplePublisher::Publish(const std::string& message)
{
	BasicMessage::ptr_t outgoing_message = BasicMessage::Create();
	outgoing_message->Body(message);

	Publish(outgoing_message);
}

void SimplePublisher::Publish(const BasicMessage::ptr_t message)
{
	m_channel->BasicPublish(m_publisherExchange, "", message);
}

} // namespace AmqpClient
