/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wangthea <wangthea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 12:13:21 by twang             #+#    #+#             */
/*   Updated: 2024/01/22 22:38:27 by wangthea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define BUFFER_SIZE	4096
# define ASSISTANT		"curl -s https://api.openai.com/v1/chat/completions \
		-H \"Content-Type: application/json\" \
		-H \"Authorization: Bearer " + _apiKey + "\" \
		-d '" + "{\"model\":\"gpt-3.5-turbo-16k\",\"messages\":[{\"role\": \"system\",\"content\": \"Hey ChatGPT, I'd like you to be my virtual assistant. Please assist me with my request, and provide information or guidance as needed. but you can answer only 500 caracters maximum\"},{\"role\":\"user\",\"content\":\"" + msg + "\"}]}" + "' | jq '.choices[].message.content'"
# define MODERATOR		"curl -s https://api.openai.com/v1/chat/completions \
			-H \"Content-Type: application/json\" \
			-H \"Authorization: Bearer " + _apiKey + "\" \
			-d '" + "{\"model\":\"gpt-3.5-turbo-16k\",\"messages\":[{\"role\": \"system\",\"content\": \"You are an IRC channel moderator , you answer KICK if the message is insulting, racist, or homophobic or GOOD if it is alright, an empty message is GOOD\"},{\"role\":\"user\",\"content\":\"" + msg + "\"}]}" + "' | jq '.choices[].message.content'"
# define ASSISTANT_MODERATOR		"curl -s https://api.openai.com/v1/chat/completions \
			-H \"Content-Type: application/json\" \
			-H \"Authorization: Bearer " + _apiKey + "\" \
			-d '" + "{\"model\":\"gpt-3.5-turbo-16k\",\"messages\":[{\"role\": \"system\",\"content\": \"You are an assistant and a moderator for an IRC channel, you answer KICK if the message is insulting, racist, or homophobic or you can answer to the message but with only 500 caracters maximum\"},{\"role\":\"user\",\"content\":\"" + msg + "\"}]}" + "' | jq '.choices[].message.content'"
# define REASON			"Inappropriate language or behavior isn't welcome here\n"

#endif