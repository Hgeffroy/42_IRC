/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 12:13:21 by twang             #+#    #+#             */
/*   Updated: 2024/01/08 14:24:08 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define BUFFER_SIZE	4096
# define ASSISTANT		"curl -s https://api.openai.com/v1/chat/completions \
		-H \"Content-Type: application/json\" \
		-H \"Authorization: Bearer " + _apiKey + "\" \
		-d '" + "{\"model\":\"gpt-3.5-turbo-16k\",\"messages\":[{\"role\": \"system\",\"content\": \"You are my assistant that , but you can answer only 500 caracters maximum\"},{\"role\":\"user\",\"content\":\"" + msg + "\"}]}" + "' | jq '.choices[].message.content'"
# define MODERATOR		""

#endif