internal void build_compile_msvc(Build_Info *info)
{
    str8_list_pushf(info->arena, &info->cmd, "setup_x64.bat & cl.exe %s", info->entry_point.cstr);
    if (info->flags & Build_Flag_DryRun)
    {
        str8_list_pushf(info->arena, &info->cmd, " -Zs");
    }
    // ak: looks
    str8_list_pushf(info->arena, &info->cmd, " -nologo -diagnostics:caret");
    // ak: output
    str8_list_pushf(info->arena, &info->cmd, " -Fo:"BUILD_DIR"\\ -Fe:");
    str8_list_push(info->arena, &info->cmd, build_path(info));
    // ak: debug
    if (info->type != Build_Type_Release)
    {
        str8_list_pushf(info->arena, &info->cmd, " -Zi -Fd\""BUILD_DIR"\\vc140.pbd\" -DBUILD_DEBUG=1");
    }
    // ak: lock lang Version
    str8_list_pushf(info->arena, &info->cmd, " -std:c11");
    // ak: Optimization
    if (info->type == Build_Type_Release)
    {
        str8_list_pushf(info->arena, &info->cmd, " -Ox -wd4711"); // Enable
    }
    else
    {
        str8_list_pushf(info->arena, &info->cmd,
            " -Od"
            " -Ob1 -wd4710" // ak: Disable inline functions and it's warnings
        );
    }
    // ak: warnings
    if (info->type != Build_Type_Release)
    {
        str8_list_pushf(info->arena, &info->cmd, " -W4 -Wall");
    }
    // ak: disable uselss warnings
    str8_list_pushf(info->arena, &info->cmd,
        " -wd4668"                 // ak: For macros magic
        " -wd4464"                 // ak: Warning about '..' in path
        " -wd4310 -wd4146 -wd4245" // ak: Cast conversion
        " -wd4201"                 // ak: Nameless struct/union
        " -wd4820"                 // ak: Struct padding
        " -wd4061"                 // ak: Enum switch enumeration
    );
    // ak: security
    str8_list_pushf(info->arena, &info->cmd,
        " -Qspectre -wd5045"  // ak: Spectre variant 1 vulnerability
        " -GS"                // ak: Canary insertion
        " -guard:cf"          // ak: Control-flow protection
    );
    if (info->type != Build_Type_Debug && info->type != Build_Type_Release)
    {
        str8_list_pushf(info->arena, &info->cmd, " -fsanitize=address");
    }
}
