import os
from openai import OpenAI
from dotenv import load_dotenv

load_dotenv()
api_key = os.getenv("OPENAI_API_KEY")

client = OpenAI(api_key=api_key)

def get_gpt_response(user_query):
    completion = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "system", "content": "You are a scavenger hunt robot."},
            {"role": "user", "content": user_query}
        ]
    )
    return completion.choices[0].message

if __name__ == "__main__":
    with open("data.txt", "r") as file:
        questions = file.readlines()
    
    continue_game = True
    current_question_index = 0

    while continue_game:
        question = questions[current_question_index].strip()
        user_input = input(f"Q: {question}\nContinue? (Y/N): ").lower()

        # Get response from ChatGPT
        response = get_gpt_response(question)

        print("ChatGPT:", response)

        if user_input in ["n", "no"]:
            continue_game = False
        elif user_input in ["y", "yes"]:
            current_question_index += 1
            if current_question_index >= len(questions):
                print("No more questions.")
                continue_game = False
