{
    "targets": [
        {
            "target_name": "spam.cp36-win32",
            "product_extension":"pyd",
            "product_dir":".",
            "type": "shared_library",
            "sources": [
                "./spammodule.c"
            ], "include_dirs": [
                "D:/Python36-32/include"
            ],
           'link_settings': {

                'libraries': [

                'D:/Python36-32/libs/python36.lib',

                ],

            },

        }
    ]
}