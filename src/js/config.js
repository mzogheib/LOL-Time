module.exports = [
    {
        "type": "heading",
        "defaultValue": "LOL Time Configuration"
    },
    {
        "type": "section",
        "capabilities": ["BW"],
        "items": [
            {
                "type": "text",
                "defaultValue": "No settings for B&W Pebbles!"
            },
            {
                "type": "submit",
                "defaultValue": "Close"
            }
        ]
    },
    {
        "type": "section",
        "capabilities": ["COLOR"],
        "items": [
            {
                "type": "radiogroup",
                "messageKey": "CHOSEN_COLOR",
                "defaultValue": "-1",
                "label": "Choose a colour",
                "options": [
                    {
                        "label": "Random",
                        "value": "-1"
                    },
                    {
                        "label": "Black",
                        "value": "0"
                    },
                    {
                        "label": "Red",
                        "value": "1"
                    },
                    {
                        "label": "Fashion Magenta",
                        "value": "2"
                    },
                    {
                        "label": "Orange",
                        "value": "3"
                    },
                    {
                        "label": "Islamic Green",
                        "value": "4"
                    },
                    {
                        "label": "Blue",
                        "value": "5"
                    },
                    {
                        "label": "Imperial Purple",
                        "value": "6"
                    }
                ]
            },
            {
                "type": "submit",
                "defaultValue": "Save Settings"
            }
        ]
    }
]